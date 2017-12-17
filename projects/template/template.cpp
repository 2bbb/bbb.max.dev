#include <string>
#include "maxcpp6.h"

class TemplateObject : public MaxCpp6<TemplateObject> {
    static void print_info() {
        static bool isnt_printed = true;
        if(isnt_printed) {
            post("bbb.template:\n");
            isnt_printed = false;
        }
    }
    
    inline void set_annotation() {
        for(std::size_t i = 0; i < 4; i++) {
            
        }
    }
    
    inline void dump(const std::string &str) {
        post("bbb.template: %s\n", str.c_str());
    }
    
    inline void dump_error(const std::string &str) {
        error("bbb.template: %s\n", str.c_str());
    }
    
public:
    TemplateObject() {
        setupIO(1, 1); // inlets / outlets
        print_info();
        dump("bbb.template: will initialize...");
    }
    
	TemplateObject(t_symbol *sym, long ac, t_atom *av)
        : TemplateObject()
    {}
	
    ~TemplateObject() {}
    
	// methods:
    void test(long inlet, t_symbol *s, long ac, t_atom *av) {
        outlet_anything(m_outlets[0], s, ac, av);
    }
    
    void assist(void *b, long io, long index, char *s) {
        switch (io) {
            case 1:
                switch(index) {
                    case 0:
                        strncpy_zero(s, "input", 32);
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                switch(index) {
                    case 0:
                        strncpy_zero(s, "output data", 32);
                        break;
                    default:
                        break;
                }
                break;
        }
    }
};

C74_EXPORT int main(void) {
	// create a class with the given name:
	TemplateObject::makeMaxClass("template");
    REGISTER_METHOD_GIMME(TemplateObject, test);
    REGISTER_METHOD_ASSIST(TemplateObject, assist);
}
