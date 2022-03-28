#include "activity.h"
#include <rice/rice.hpp>
#include "discord.h"
#include "common.h"
#include <cstdint>
using namespace Rice;

Class rb_cActivity;

DEF_TYPE_CUSTOMTYPENAME(discord::Activity, Activity_rbType);

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

std::int64_t rb_activity_timestamp_get_timestamp_start(Object self) {
    GET_ACTIVITY(self);

    return activity->GetTimestamps().GetStart();
}

Object rb_activity_timestamp_set_timestamp_start(Object self, std::int64_t start) {
    GET_ACTIVITY(self);

    activity->GetTimestamps().SetStart(start);

    return Qnil;
}

std::int64_t rb_activity_timestamp_get_timestamp_end(Object self) {
    GET_ACTIVITY(self);

    return activity->GetTimestamps().GetEnd();
}

Object rb_activity_timestamp_set_timestamp_end(Object self, std::int64_t end) {
    GET_ACTIVITY(self);

    activity->GetTimestamps().SetEnd(end);

    return Qnil;
}

const char* rb_activity_assets_get_large_image(Object self) {
    GET_ACTIVITY(self);

    return activity->GetAssets().GetLargeImage();
}

Object rb_activity_assets_set_large_image(Object self, const char* largeImage) {
    GET_ACTIVITY(self);

    activity->GetAssets().SetLargeImage(largeImage);

    return Qnil;
}

const char* rb_activity_assets_get_large_text(Object self) {
    GET_ACTIVITY(self);

    return activity->GetAssets().GetLargeText();
}

Object rb_activity_assets_set_large_text(Object self, const char* largeText) {
    GET_ACTIVITY(self);

    activity->GetAssets().SetLargeText(largeText);

    return Qnil;
}

const char* rb_activity_assets_get_small_image(Object self) {
    GET_ACTIVITY(self);

    return activity->GetAssets().GetSmallImage();
}

Object rb_activity_assets_set_small_image(Object self, const char* smallImage) {
    GET_ACTIVITY(self);

    activity->GetAssets().SetSmallImage(smallImage);

    return Qnil;
}

const char* rb_activity_assets_get_small_text(Object self) {
    GET_ACTIVITY(self);

    return activity->GetAssets().GetSmallText();
}

Object rb_activity_assets_set_small_text(Object self, const char* smallText) {
    GET_ACTIVITY(self);

    activity->GetAssets().SetSmallText(smallText);

    return Qnil;
}

const char* rb_activity_party_get_party_id(Object self) {
    GET_ACTIVITY(self);

    return activity->GetParty().GetId();
}

Object rb_activity_party_set_party_id(Object self, const char* id) {
    GET_ACTIVITY(self);

    activity->GetParty().SetId(id);

    return Qnil;
}

std::int32_t rb_activity_party_get_party_size(Object self) {
    GET_ACTIVITY(self);

    return activity->GetParty().GetSize().GetCurrentSize();
}

Object rb_activity_party_set_party_size(Object self, std::int32_t size) {
    GET_ACTIVITY(self);

    activity->GetParty().GetSize().SetCurrentSize(size);

    return Qnil;
}

std::int32_t rb_activity_party_get_party_max(Object self) {
    GET_ACTIVITY(self);

    return activity->GetParty().GetSize().GetMaxSize();
}

Object rb_activity_party_set_party_max(Object self, std::int32_t max) {
    GET_ACTIVITY(self);

    activity->GetParty().GetSize().SetMaxSize(max);

    return Qnil;
}

const char* rb_activity_secrets_get_secret_match(Object self) {
    GET_ACTIVITY(self);

    return activity->GetSecrets().GetMatch();
}

Object rb_activity_secrets_set_secret_match(Object self, const char* match) {
    GET_ACTIVITY(self);

    activity->GetSecrets().SetMatch(match);

    return Qnil;
}

const char* rb_activity_secrets_get_secret_join(Object self) {
    GET_ACTIVITY(self);

    return activity->GetSecrets().GetJoin();
}

Object rb_activity_secrets_set_secret_join(Object self, const char* join) {
    GET_ACTIVITY(self);

    activity->GetSecrets().SetJoin(join);

    return Qnil;
}

const char* rb_activity_secrets_get_secret_spectate(Object self) {
    GET_ACTIVITY(self);

    return activity->GetSecrets().GetSpectate();
}

Object rb_activity_secrets_set_secret_spectate(Object self, const char* spectate) {
    GET_ACTIVITY(self);

    activity->GetSecrets().SetSpectate(spectate);

    return Qnil;
}

void rb_activity_init_class(Module module) {
    rb_cActivity = define_class_under(module, "Activity");
    rb_cActivity.define_method("initialize", &rb_activity_initialize);
    rb_define_alloc_func(rb_cActivity, classAllocate<&Activity_rbType>);
    DEF_METHOD_GET_SET(rb_cActivity, type, activity);
    DEF_METHOD_GET_SET(rb_cActivity, application_id, activity);
    DEF_METHOD_GET_SET(rb_cActivity, name, activity);
    DEF_METHOD_GET_SET(rb_cActivity, state, activity);
    DEF_METHOD_GET_SET(rb_cActivity, details, activity);
    DEF_METHOD_GET_SET(rb_cActivity, instance, activity);
    // Timestamps
    DEF_METHOD_GET_SET(rb_cActivity, timestamp_start, activity_timestamp);
    DEF_METHOD_GET_SET(rb_cActivity, timestamp_end, activity_timestamp);
    // Assets
    DEF_METHOD_GET_SET(rb_cActivity, large_image, activity_assets);
    DEF_METHOD_GET_SET(rb_cActivity, large_text, activity_assets);
    DEF_METHOD_GET_SET(rb_cActivity, small_image, activity_assets);
    DEF_METHOD_GET_SET(rb_cActivity, small_text, activity_assets);
    // Party
    DEF_METHOD_GET_SET(rb_cActivity, party_id, activity_party);
    DEF_METHOD_GET_SET(rb_cActivity, party_size, activity_party);
    DEF_METHOD_GET_SET(rb_cActivity, party_max, activity_party);
    // Secrets
    DEF_METHOD_GET_SET(rb_cActivity, secret_match, activity_secrets);
    DEF_METHOD_GET_SET(rb_cActivity, secret_join, activity_secrets);
    DEF_METHOD_GET_SET(rb_cActivity, secret_spectate, activity_secrets);
}