require 'ostruct'

config_file = Rails.root + 'config/config.yml'

options = YAML.load_file(config_file)
if !options[Rails.env]
  raise "'#{Rails.env}' was not found in #{config_file}"
end

AppConfig = OpenStruct.new(options[Rails.env])
