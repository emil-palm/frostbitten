module Frostbitten
	module Version
    	MAJOR = 0
    	MINOR = 0
    	PATCH = 2
    	BUILD = 'pre1'

    	STRING = [MAJOR, MINOR, PATCH, BUILD].compact.join('.')
    	def self.string
			"Frostbitten version #{Frostbitten::Version::STRING}"
		end
    end

    
end