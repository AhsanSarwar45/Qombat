#include <QMBT.hpp>

class Editor : public QMBT::Application
{
public:
    Editor()
        : Application("QCreate")
    {
    }
    ~Editor()
    {
    }
};

QMBT::Application *QMBT::CreateApplication()
{
    return new Editor();
}