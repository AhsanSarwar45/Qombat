#pragma once

#include "QMBTPCH.hpp"

#include "Core/Aliases.hpp"
#include "Core/Logging/Logger.hpp"
#include "Core/Macros.hpp"

namespace QMBT
{

	enum class ProfileCategory : UInt8
	{
		Layers,
		Rendering,
		Physics,
		Other
	};

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileData
	{
		std::string Name;
		ProfileCategory Category;
		double StartTime;
		double EndTime;
		double ElapsedTime;
		std::thread::id ThreadID;
	};

	struct Frame
	{
		std::vector<Ref<ProfileData>> Data;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	using TimesArray = std::array<std::vector<float>, static_cast<int>(ProfileCategory::Other) + 1>;

	// Singleton class
	class Instrumentor
	{
	  public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession()
		{
			std::lock_guard<std::mutex> lock(m_Mutex);

			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Logger::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					LOG_CORE_ERROR("Instrumentor::BeginSession() when session one already open.");
				}
				InternalEndSession();
			}

			m_CurrentSession = new InstrumentationSession();
			for (auto& vec : m_Times)
			{
				vec.clear();
			}
			m_Frames.clear();

			m_Recording = true;
			m_SessionStartTime = FloatingPointMicroseconds{std::chrono::steady_clock::now().time_since_epoch()}.count();
		}

		void EndSession()
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			InternalEndSession();
		}

		void AddProfile(ProfileData data)
		{
			data.StartTime -= m_SessionStartTime; // Normalize the time, making it the time from the start of the session
			data.EndTime -= m_SessionStartTime;
			m_Frames.back().Data.push_back(std::make_shared<ProfileData>(data));
		}

		void EndFrame()
		{
			if (m_Recording)
			{
				Frame* frame = &m_Frames.back();
				UInt64 size = m_Frames.size();

				for (auto& data : frame->Data)
				{
					// Update the time of the category of the current frame (size - 1)
					m_Times[static_cast<UInt8>(data->Category)][size - 1] += data->ElapsedTime;
				}
			}
		}

		void BeginFrame()
		{
			if (m_Recording)
			{
				m_Frames.push_back(Frame{});

				for (auto& vec : m_Times)
				{
					vec.push_back(0);
				}
			}
		}

		bool IsRecording() const { return m_Recording; }

		void Pause() { m_Recording = false; }

		std::vector<Frame>* GetFrames()
		{
			return &m_Frames;
		}

		TimesArray* GetTimes() { return &m_Times; }

		static Instrumentor&
		Get()
		{
			static Instrumentor instance;
			return instance;
		}

	  private:
		Instrumentor()
			: m_CurrentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
				m_Recording = false;
			}
		}

	  private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		TimesArray m_Times;
		std::vector<Frame> m_Frames;
		double m_SessionStartTime;
		bool m_Recording;
	};

	class InstrumentationTimer
	{
	  public:
		InstrumentationTimer(const char* name, ProfileCategory category = ProfileCategory::Other)
			: m_Name(name), m_Category(category), m_Stopped(false)
		{
			m_StartTime = FloatingPointMicroseconds{std::chrono::steady_clock::now().time_since_epoch()}.count();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
			{
				double endTime = FloatingPointMicroseconds{std::chrono::steady_clock::now().time_since_epoch()}.count();
				Instrumentor::Get()
					.AddProfile({
						m_Name,
						m_Category,
						m_StartTime,
						endTime,
						endTime - m_StartTime,
						std::this_thread::get_id(),
					});
				m_Stopped = true;
			}
		}

	  private:
		bool m_Stopped;
		const char* m_Name;
		double m_StartTime;
		ProfileCategory m_Category;
	};

	namespace InstrumentorUtils
	{

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	} // namespace InstrumentorUtils
} // namespace QMBT

#define QMBT_PROFILE 1
#if QMBT_PROFILE

#define PROFILE_SCOPE_LINE_INTERNAL2(name, line)                                                           \
                                                                                                           \
	if (::QMBT::Instrumentor::Get().IsRecording())                                                         \
	{                                                                                                      \
		constexpr auto fixedName##line = ::QMBT::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
		::QMBT::InstrumentationTimer timer##line(fixedName##line.Data);                                    \
	}
#define PROFILE_SCOPE_LINE_INTERNAL3(name, line, group)                                                    \
                                                                                                           \
	if (::QMBT::Instrumentor::Get().IsRecording())                                                         \
	{                                                                                                      \
		constexpr auto fixedName##line = ::QMBT::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
		::QMBT::InstrumentationTimer timer##line(fixedName##line.Data, group);                             \
	}

#define PROFILE_SCOPE_LINE_INTERNAL(...)                                                 \
	GET_MACRO_3(__VA_ARGS__, PROFILE_SCOPE_LINE_INTERNAL3, PROFILE_SCOPE_LINE_INTERNAL2) \
	(__VA_ARGS__)

#define PROFILE_SCOPE_LINE2(name, line) PROFILE_SCOPE_LINE_INTERNAL(name, line)
#define PROFILE_SCOPE_LINE3(name, line, group) PROFILE_SCOPE_LINE_INTERNAL(name, line, group)
#define PROFILE_SCOPE_LINE(...)                                        \
	GET_MACRO_3(__VA_ARGS__, PROFILE_SCOPE_LINE3, PROFILE_SCOPE_LINE2) \
	(__VA_ARGS__)

#define PROFILE_SCOPE2(name) PROFILE_SCOPE_LINE(name, __LINE__)
#define PROFILE_SCOPE3(name, group) PROFILE_SCOPE_LINE(name, __LINE__, group)
#define PROFILE_SCOPE(...)                                   \
	GET_MACRO_2(__VA_ARGS__, PROFILE_SCOPE3, PROFILE_SCOPE2) \
	(__VA_ARGS__)

#define PROFILE_FUNCTION2() PROFILE_SCOPE(QMBT_FUNC_SIG)
#define PROFILE_FUNCTION3(group) PROFILE_SCOPE(QMBT_FUNC_SIG, group)
#define PROFILE_FUNCTION(...)                                                        \
	GET_MACRO_1(_0 __VA_OPT__(, ) __VA_ARGS__, PROFILE_FUNCTION3, PROFILE_FUNCTION2) \
	(__VA_ARGS__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()

#endif
