#include <iostream>
#include <dlfcn.h>
#include <unistd.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

inline bool file_exists (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

int main(int argc, char* argv[]) {

    void *lib_handle = nullptr;

    if (file_exists("./libpizdryk.so"))
    {
        lib_handle = dlopen("./libpizdryk.so", RTLD_LAZY);
    }
    
    if (!lib_handle)
    {
        std::cout<< "Missing libpizdryk" << std::endl;
    }

    typedef void (*list_widgets_funcptr)();
    list_widgets_funcptr list_widgets = nullptr;
    typedef void (*call_slot_funcptr)(std::string, std::string);
    call_slot_funcptr call_slot = nullptr;

    if (lib_handle)
    {
        list_widgets = reinterpret_cast<list_widgets_funcptr> (dlsym(lib_handle, "list_widgets"));
        call_slot = reinterpret_cast<call_slot_funcptr> (dlsym(lib_handle, "call_slot"));
    }
    
    if (dlerror() != NULL) 
    {
        exit(1);
    }

    QApplication app(argc, argv);
    QPushButton hello( "Hello world!", 0 );
    QPushButton callbutton( "call", 0 );
    hello.resize( 100, 30 );
    callbutton.resize(100,30);
    hello.show();
    callbutton.show();
    hello.setObjectName("baton");

    if (list_widgets)
    {
        list_widgets();
    }

    QObject::connect(&hello, &QPushButton::clicked, [&] () 
        {
            std::cout << "Button clicked" << std::endl;
            app.quit();
        });
    QObject::connect(&callbutton, &QPushButton::clicked, [&] () 
        {
            if (call_slot)
            {
                call_slot("baton","clicked");
            }
        });
    app.exec();

    if (lib_handle)
    {
        dlclose(lib_handle);
    }
}
