require 'hashie'

module Frostbitten
	# Class to repesent a player object returned by server
	class Player < Hashie::Trash
		property :name
		property :guid
		property :team_id, :from => :teamId
		property :squad_id, :from => :squadId
		property :kills
		property :deaths
		property :score
	end
end