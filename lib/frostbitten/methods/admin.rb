module Frostbitten
	module Methods
		module AdminCommands
			def map_list(start_index=0)
				return nil unless @logged_in 
				return Map.maps_from_list(send(['mapList.list', "#{start_index}"]))
			end
		end
	end
end