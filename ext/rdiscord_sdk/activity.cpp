#include "activity.h"
#include <rice/rice.hpp>
#include "discord.h"
using namespace Rice;

Class rb_cActivity;

// https://jasonroelofs.com/rice/4.x/advanced/type_conversions.html

namespace Rice::detail {
    template<>
    struct Type<discord::ActivityType>
    {
        static bool verify() {
            return true;
        }
    } 
    template<>
    class To_Ruby<discord::ActivityType>
    {
        public:
        VALUE convert(const discord::ActivityType& type)
        {
            VALUE res;
        }
    }
}

void rb_activity_init_class(Module module) {
    rb_cActivity = define_class_under<discord::Activity>(module, "Activity") 
                                                .define_method("SetType", &discord::Activity::SetType); 
}