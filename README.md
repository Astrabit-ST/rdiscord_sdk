# R(uby) Discord SDK ![Gem](https://img.shields.io/gem/v/rdiscord_sdk) ![Gem](https://img.shields.io/gem/dt/rdiscord_sdk)

This is a Ruby gem that implements bindings to the [Discord Game SDK](https://discord.com/developers/docs/game-sdk/discord).
It is intended to be used in [ModShot](https://github.com/Astrabit-ST/Modshot-Core), although will work in any ruby application.

## Whats the difference between this and ruby_discord_game_sdk?

Well, [ruby_discord_game_sdk](https://github.com/rkevin-arch/ruby_discord_game_sdk) is written in pure C, making it very hard to understand and interpret.
This means it's very difficult to understand how it works, what it does, and to add new features. 
rdiscord_sdk is instead built in C++ meaning it's easier to abstract, and to modify the source. The name may be obtuse, but it sure as hell isn't internally.
My goal is to provide extra levels of abstraction over ruby_discord_game_sdk, and provide full functionality as well. Wether that will happen, I'm not sure.

## Disclaimer
Some portions of code are untested because they can be extremely hard to test, namely events.
Keep that in mind.

## Usage

This gem (at the moment) exposes SDK functions without much abstraction, so as long as you can grasp C#, you should be able to transfer the example code to ruby code.
There are a couple of minor differences, such as the lack of a need to initialize managers (C++ Discord SDK bindings automatically do that for you).

- Setting up the SDK is the same as ruby_discord_game_sdk `Discord::init(client_id, flags)`
- Enums are just constants inside lib/rdiscord_sdk/enums.rb (So `Discord.Result.Ok` ~> `Discord::Result::Ok`)
- The module for SDK functions is under `Discord`, not `DiscordSDK`
- Callbacks are method blocks
- Events are also method blocks
- Pending callbacks and events are accessible
- Some functions return an array, usually the result being at [0] and an object at [1] (such as Discord::UserManager.get_current_user)
- Images will likely not be supported on many platforms aside from mkxp-z, as mkxp-z has the ability to create bitmaps from strings
- Some network manager events and methods return/take in an array of bytes (integers from 0-255, similar to a byte array in C#) rather than how you'd expect in C++

As mentioned above, some functions return arrays. An easy way to deal with these would be to:
```rb
result, user = Discord::UserManager.get_current_user
```
It's not the best, but it works. I don't want to discard a result, in case you ever want to know what went wrong.
If you don't want the result, you can just do:
```rb
_, user = Discord::UserManager.get_current_user
```
`_` is widely understood as a dummy variable, so it's good practice to use it when you don't care about a specific return value.

## Callbacks & Events

As mentioned before, callbacks and events are method blocks and procs, and are not usually required. If you've coded ruby before, just use them how you normally would.
Generally, when you see `() => {}` in the SDK documentation, that is referring to a callback or event, and so you should substitute it with a block instead.

Docs:
```cs
var userManager = discord.GetUserManager();
var activity = new Discord.Activity
activityManager.UpdateActivity(activity, (result) =>
{
    if (result == Discord.Result.Ok)
    {
        Console.WriteLine("Success!");
    }
    else
    {
        Console.WriteLine("Failed");
    }
}
```
rdiscord_sdk equivalent:
```rb
Discord::ActivityManager.update_activity(Discord::Activity.new) do |result|
    if result == Discord::Result::Ok
        puts "Success!"
    else
        puts "Failed"
    end
end
```

If no block is provided, and an error occurs, it should be logged to stderr. (or your log hook if that is set up)

Events work a little bit differently than in the SDK docs. As far as I can tell, the docs don't cover "connecting" or "disconnecting".
Events require you to "connect" to them, meaning you can subscribe multiple blocks to any single event. (iirc the cap is 4?)
When you add something to an event, it will return a token, an identifier to the block passed into the event, which **must** be used when removing blocks from an event.
It's sort of like a subscriber system, where things can subscribe to an event, and then unsubscribe from said event at any point.

Events and pending callbacks are stored in a global array and hash respectively, if you ever need to access them for some reason. 
**I would not suggest trying to modify them.** It will inevitably cause bad problems.

C# code:
```cs
activityManager.OnActivityJoin += secret => {
    Console.WriteLine("OnJoin {0}", secret);
}
```
rdiscord_sdk equivalent:
```rb
token = Discord::ActivityManager.on_activity_join_connect do |secret|
            puts "OnJoin #{secret}"
        end

Discord::ActivtyManager.on_activity_join_disconnect(token) # I'm not sure what the C# equivalent is
```

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'rdiscord_sdk'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install rdiscord_sdk


## Development

After checking out the repo, run `bin/setup` to install dependencies. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and the created tag, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## License

The gem is available as open source under the terms of the [GPL-3]() license.

This gem also uses the [Discord SDK](https://discord.com/developers/docs/game-sdk/discord), which is proprietary. Everything under `third_party/` is owned by Discord.

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/Speak2Erase/rdiscord_sdk.

## Development Progress

### Data Classes

- [x] DiscordUser
- [ ] DiscordOAuth2Token
- [ ] DiscordImageHandle
- [ ] DiscordImageDimensions
- [x] DiscordActivityTimestamps
- [x] DiscordActivityAssets
- [x] DiscordPartySize
- [x] DiscordActivityParty
- [x] DiscordActivitySecrets
- [x] DiscordActivity
- [ ] DiscordPresence
- [ ] DiscordRelationship
- [ ] DiscordLobby
- [ ] DiscordFileStat
- [ ] DiscordEntitlement
- [ ] DiscordSkuPrive
- [ ] DiscordSku
- [ ] DiscordInputMode
- [ ] DiscordUserAchievement

### Managers implemented
- [ ] ApplicationManager
- [x] UserManager
- [ ] ImageManager
- [x] ActivityManager
- [ ] RelationshipManager
- [ ] LobbyManager
- [x] NetworkManager
- [x] OverlayManager
- [ ] StorageManager
- [ ] StoreManager
- [ ] VoiceManager
- [ ] AchievementManager

### Others
- [x] Core functionality
  - [x] Initialization
  - [x] Run callbacks
  - [x] Log hooks
- [x] All enums
- [ ] DiscordLobbyTransaction
- [ ] DiscordLobbySearchQuery
- [x] Events
  - [x] UserEvents
  - [x] ActivityEvents
  - [ ] RelationshipEvents
  - [ ] LobbyEvents
  - [x] NetworkEvents
  - [x] OverlayEvents
  - [ ] StoreEvents
  - [ ] VoiceEvents
  - [ ] AchievementEvents
