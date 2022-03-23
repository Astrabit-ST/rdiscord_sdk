require "mkmf-rice"

# Add compiler and linker flags
def add_flags(type, flags)
  case type
  when :c
    $CFLAGS << " #{flags} "
  when :ld
    $LDFLAGS << " #{flags} "
  when :inc
    $INCFLAGS << " #{flags} "
  end
end

THIRDPARTY_DIR = File.join(File.dirname(__FILE__), "../../third-party")
puts "THIRDPARTY_DIR: #{THIRDPARTY_DIR}"
HEADER_DIR = File.join(THIRDPARTY_DIR, "include")
LIBRARY_DIR = if RUBY_PLATFORM.start_with?("x86_64") or RUBY_PLATFORM.start_with?("x64")
    File.join(THIRDPARTY_DIR, "lib/x86_64")
  else
    puts "Blindly assuming x86. If you get linker errors, this is why."
    File.join(THIRDPARTY_DIR, "lib/x86")
  end

add_flags(:ld, "-L#{LIBRARY_DIR}")
add_flags(:ld, "-Wl,-R. -Wl,-R./lib")
add_flags(:ld, "-l:discord_game_sdk#{RUBY_PLATFORM.end_with?("linux") ? ".so" : ".dll"}")
add_flags(:inc, "-I#{HEADER_DIR}")
$LDFLAGS.gsub!(/\n/, " ")

puts "LDFLAGS: #{$LDFLAGS}"
puts "CFLAGS: #{$CFLAGS}"

create_makefile("rdiscord_sdk")
