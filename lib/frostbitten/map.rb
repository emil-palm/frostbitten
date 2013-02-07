require 'hashie'

module Frostbitten
	class Map < Hashie::Trash
		class << self
			def maps_from_list(list,index=0)
				number_of_maps = list.slice!(index).to_i
				number_of_words_per_map = list.slice!(index).to_i
				columns = number_of_maps*number_of_words_per_map
				return [].tap do |maps|
					list.slice(index,columns).each_slice(number_of_words_per_map) do |map_data|
						maps << Map.new(map_data)
					end
				end
			end
		end

		property :name
		property :gamemode
		property :round_count

		def initialize(data,gamemode=nil,round_count=nil)
			if data.is_a? Array
				self.name = data[0]
				self.gamemode = data[1]
				self.round_count = data[2]
			elsif data.is_a? Hash
				self.name = data["name"]
				self.gamemode = data["gamemode"]
				self.round_count = data["round_count"]
			elsif data.is_a? String
				self.name = data
				self.gamemode = gamemode
				self.round_count =  round_count
			end
		end
	end
end
