require 'hashie'

module Frostbitten
	# Class to repesent a player object returned by server
	class Player < Hashie::Trash
		class << self
			def players_from_list(list,index)
				key_count = list.slice!(index).to_i
				keys = list.slice!(index,key_count)
				player_count = list.slice!(index).to_i

				return [].tap do |players|
					{}.tap do |player|
						if player_count > 0
							list.slice!(index,player_count*key_count).each_slice(key_count) do |player_values|
								player_values.each_with_index { |value,index| player[keys[index]] = value }
								players << Player.new(player)
							end
						end
					end
				end
			end
		end

		property :name
		property :guid
		property :team_id, :from => :teamId, :with => lambda { |v| v.to_i }
		property :squad_id, :from => :squadId, :with => lambda { |v| v.to_i }
		property :kills, :transform_with => lambda { |v| v.to_i }
		property :deaths, :transform_with => lambda { |v| v.to_i }
		property :score, :transform_with => lambda { |v| v.to_i }
	end
end