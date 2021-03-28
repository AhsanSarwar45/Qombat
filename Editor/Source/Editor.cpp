#include <QMBT.hpp>

class Editor : public QMBT::Application
{
public:
    Editor()
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