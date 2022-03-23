require "mkmf-rice"

THIRDPARTY_DIR = File.realpath(File.join(File.dirname(__FILE__), "../../third-party"))
puts "THIRDPARTY_DIR: #{THIRDPARTY_DIR}"
HEADERS = File.join(THIRDPARTY_DIR, "include")
LIBRARY_DIR = if RUBY_PLATFORM.start_with?("x86_64") or RUBY_PLATFORM.start_with?("x64")
    File.join(THIRDPARTY_DIR, "lib/x86_64")
  else
    puts "Blindly assuming x86. If you get linker errors, this is why."
    File.join(THIRDPARTY_DIR, "lib/x86")
  end

$LDFLAGS += " -L #{LIBRARY_DIR} -Wl,-R. -Wl,-R./lib -l:discord_game_sdk#{RUBY_PLATFORM.end_with?("linux") ? ".so" : ".dll"}"

create_makefile("rdiscord_sdk")
