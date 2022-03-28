#include "user.h"
#include <rice/rice.hpp>
#include "discord.h"
#include "common.h"
#include <cstdint>
using namespace Rice;

Class rb_cUser;

DEF_TYPE_CUSTOMTYPENAME(discord::User, User_rbType);

Object rb_user_initialize(Object self) {
    discord::User* user = new discord::User();
    setPrivateData(self, user);

    return self;
}

std::int64_t rb_user_get_id(Object self) {
    GET_USER(self);

    return user->GetId();
}

Object rb_user_set_id(Object self, std::int64_t id) {
    GET_USER(self);

    user->SetId(id);

    return Qnil;
}

const char* rb_user_get_username(Object self) {
    GET_USER(self);

    return user->GetUsername();
}

Object rb_user_set_username(Object self, const char* username) {
    GET_USER(self);

    user->SetUsername(username);

    return Qnil;
}

const char* rb_user_get_discriminator(Object self) {
    GET_USER(self);

    return user->GetDiscriminator();
}

Object rb_user_set_discriminator(Object self, const char* discriminator) {
    GET_USER(self);

    user->SetDiscriminator(discriminator);

    return Qnil;
}

const char* rb_user_get_avatar(Object self) {
    GET_USER(self);

    return user->GetAvatar();
}

Object rb_user_set_avatar(Object self, const char* avatar) {
    GET_USER(self);

    user->SetAvatar(avatar);

    return Qnil;
}

bool rb_user_get_bot(Object self) {
    GET_USER(self);

    return user->GetBot();
}

Object rb_user_set_bot(Object self, bool bot) {
    GET_USER(self);

    user->SetBot(bot);

    return Qnil;
}

void rb_user_init_class(Module module) {
    rb_cUser = define_class_under(module, "User");
    rb_define_alloc_func(rb_cUser, classAllocate<&User_rbType>);
    rb_cUser.define_method("initialize", &rb_user_initialize);
    DEF_METHOD_GET_SET(rb_cUser, id, user);
    DEF_METHOD_GET_SET(rb_cUser, username, user);
    DEF_METHOD_GET_SET(rb_cUser, discriminator, user);
    DEF_METHOD_GET_SET(rb_cUser, avatar, user);
    DEF_METHOD_GET_SET(rb_cUser, bot, user);
}