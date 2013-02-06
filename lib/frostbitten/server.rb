require 'hashie'
module Frostbitten
	class Server < Hashie::Trash
		property :name, :from => "data1"
		property :playercount, :from => "data2"
		property :playermax, :from => "data3"
		property :playmode, :from => "data4"
		property :map, :from => "data5"
		property :played_rounds, :from => "data6"
		property :total_rounds, :from => "data7"
		property :scores, :from => "data8"
		property :end_score, :from => "data9"
		property :online_state, :from => "data10"
		property :ranked, :from => "data11"
		property :punkbuster, :from => "data12"
		property :password, :from => "data13"
		property :uptime, :from => "data14"
		property :round_time, :from => "data15"
		property :ip_port, :from => "data16"
		property :punkbuster_version, :from => "data17"
		property :join_queue_enabled, :from => "data18"
		property :region, :from => "data19"
		property :ping_site, :from => "data20"
		property :country, :from => "data21"
		property :match_making_enabled, :from => "data22"
	end
end