require "mkmf-rice"

# Add compiler and linker flags
def add_flags(type, flags)
  case type
  when :c
    $CFLAGS << " #{flags} "
  when :ld
    $LDFLAGS << " #{flags} "
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
add_flags(:ld, "-l:discord_game_sdk#{RUBY_PLATFORM.end_with?("linux") ? ".so" : ".dll.lib"}")
$LDFLAGS.gsub!(/\n/, " ")

# Grab all discord source files because ruby is dumb

discord_srcs = Dir.glob("#{THIRDPARTY_DIR}/**/*.cpp").map { |path| File.basename(path) }
Dir.glob("#{THIRDPARTY_DIR}/*/") do |path|
  puts path
  dir = File.basename(path)
  $VPATH << "#{THIRDPARTY_DIR}/#{dir}"
  $INCFLAGS << " -I#{THIRDPARTY_DIR}/#{dir}"
end
ext_srcs = Dir.glob("#{$srcdir}/**/*.cpp").map { |path| File.basename(path) }
Dir.glob("#{$srcdir}/*/") do |path|
  dir = File.basename(path)
  next if excluded.include?(dir)
  $VPATH << "$(srcdir)/#{dir}"
  $INCFLAGS << " -I$(srcdir)/#{dir}"
end

$srcs = discord_srcs + ext_srcs

puts "LDFLAGS: #{$LDFLAGS}"
puts "CFLAGS: #{$CFLAGS}"
puts "VPATH: #{$VPATH}"
puts "INCFLAGS: #{$INCFLAGS}"
puts "SRCS: #{$srcs}"

create_makefile("rdiscord_sdk")
