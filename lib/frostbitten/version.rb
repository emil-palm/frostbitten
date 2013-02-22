module Frostbitten
	module Version
    	MAJOR = 0
    	MINOR = 1
    	PATCH = 0
    	BUILD = nil

    	STRING = [MAJOR, MINOR, PATCH, BUILD].compact.join('.')
    	def self.string
			"Frostbitten version #{Frostbitten::Version::STRING}"
		end
    end

    
end