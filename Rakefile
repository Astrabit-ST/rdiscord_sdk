# frozen_string_literal: true

require "bundler/gem_tasks"
require "rubocop/rake_task"
require "rake/extensiontask"

RuboCop::RakeTask.new

task default: :rubocop

task build: :compile

Rake::ExtensionTask.new("rdiscord_sdk") do |ext|
  ext.lib_dir = "lib/rdiscord_sdk"
end

task default: %i[clobber compile]
