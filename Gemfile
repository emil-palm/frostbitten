source "http://rubygems.org"
gem 'hashie', :git => "git://github.com/intridea/hashie.git"
# Add dependencies required to use your gem here.
# Example:
#   gem "activesupport", ">= 2.3.5"

# Add dependencies to develop your gem here.
# Include everything needed to run rake, tests, features, etc.
group :development do
  gem "shoulda", ">= 0"
  gem "rdoc", "~> 3.12"
  gem "bundler"
  gem "jeweler", "~> 1.8.4"
  gem 'rspec'
  gem 'rb-fsevent', :require => false if RUBY_PLATFORM =~ /darwin/i  
  gem 'guard-rspec'
  gem 'terminal-notifier-guard'
  gem 'rake-compiler'
end

