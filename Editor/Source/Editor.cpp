#include <QMBT.hpp>

class TestLayer : public QMBT::Layer
{
  public:
	TestLayer()
		: Layer("Test Layer")
	{
	}

	void OnUpdate(const QMBT::TimeStep& timeStep) override
	{
		LOG_CORE_INFO("Update App");
	}

	void OnEvent(QMBT::Event& event) override
	{
		LOG_CORE_INFO("QCreate {0}", event);
	}
};

class Editor : public QMBT::Application
{
  public:
	Editor()
		: Application("QCreate")
	{
		TestLayer* testLayer = m_LayerStackAllocator.New<TestLayer>();
		PushLayer(testLayer);
	}
	~Editor()
	{
	}
};

QMBT::Application* QMBT::CreateApplication()
{
	return new Editor();
}