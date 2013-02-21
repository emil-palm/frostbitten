module Frostbitten
	module Methods
		module AdminCommands
			def map_list(start_index=0)
				return nil unless @logged_in 
				return Map.maps_from_list(send(['mapList.list', "#{start_index}"]))
			end

			def ban_list(start_index=0)
				return nil unless @logged_in
				return BannedPlayer.players_from_list(send(['banList.list', "#{start_index}"]))
			end

			def reserved_slots_list(start_index=0)
				return nil unless @logged_in
				return [].tap do |players|
					send(['reservedSlotsList.list', "#{start_index}"]).each do |player_name|
						players << Player.new({:name => player_name})
					end
				end
			end

			def events(start=true)
				return nil unless @logged_in
				return send(['admin.eventsEnabled',(start ? 'true' : 'false')])
			end
		end
	end
end