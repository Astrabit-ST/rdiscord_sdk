#include "activity.h"
#include <rice/rice.hpp>
#include "discord.h"
#include "common.h"
#include <cstdint>
using namespace Rice;

Class rb_cActivity;

Object rb_activity_initialize(Object self) {
    discord::Activity* activity = new discord::Activity();
    setPrivateData(self, activity);

    return self;
}

int rb_activity_get_type(Object self) {
    GET_ACTIVITY(self);

    return (int) activity->GetType();
}

Object rb_activity_set_type(Object self, int type) {
    GET_ACTIVITY(self);

    activity->SetType((discord::ActivityType) type);

    return Qnil;
}

std::int64_t rb_activity_get_application_id(Object self) {
    GET_ACTIVITY(self);

    return activity->GetApplicationId();
}

Object rb_activity_set_application_id(Object self, std::int64_t id) {
    GET_ACTIVITY(self);

    activity->SetApplicationId(id);

    return Qnil;
}

const char* rb_activity_get_name(Object self) {
    GET_ACTIVITY(self);

    return activity->GetName();
}

Object rb_activity_set_name(Object self, const char* name) {
    GET_ACTIVITY(self);

    activity->SetName(name);

    return Qnil;
}

const char* rb_activity_get_state(Object self) {
    GET_ACTIVITY(self);

    return activity->GetState();
}

Object rb_activity_set_state(Object self, const char* state) {
    GET_ACTIVITY(self);

    activity->SetState(state);

    return Qnil;
}

const char* rb_activity_get_details(Object self) {
    GET_ACTIVITY(self);

    return activity->GetDetails();
}

Object rb_activity_set_details(Object self, const char* details) {
    GET_ACTIVITY(self);

    activity->SetDetails(details);

    return Qnil;
}

bool rb_activity_get_instance(Object self) {
    GET_ACTIVITY(self);

    return activity->GetInstance();
}

Object rb_activity_set_instance(Object self, bool instance) {
    GET_ACTIVITY(self);

    activity->SetInstance(instance);

    return Qnil;
}

void rb_activity_init_class(Module module) {
    rb_cActivity = define_class_under(module, "Activity");
    rb_cActivity.define_method("initialize", &rb_activity_initialize);
    DEF_METHOD_GET_SET(rb_cActivity, type, activity);
    DEF_METHOD_GET_SET(rb_cActivity, application_id, activity);
    DEF_METHOD_GET_SET(rb_cActivity, name, activity);
    DEF_METHOD_GET_SET(rb_cActivity, state, activity);
    DEF_METHOD_GET_SET(rb_cActivity, details, activity);
    DEF_METHOD_GET_SET(rb_cActivity, instance, activity);
}