$:.unshift File.dirname(__FILE__)
require 'frostbitten'
# m = Frostbitten::Message.new; 
# m.words << "asdf"; 
# m.header.sequence = 1; 
# io = File.open("/tmp/path_to_file.bin","w+b")
# m.write io

# io2 = File.open("/tmp/path_to_file.bin","rb")
# x = Frostbitten::Message.new
# x.read io2
# puts x.header.sequence
# puts x.words

hostname = '192.168.1.102'
port = 11000
require 'socket'      # Sockets are in standard library
TCPSocket.open(hostname, port) do |s|

	header = Frostbitten::Header.new({:sequence => 1,:response => false, :origin => :client})
	m = Frostbitten::Message.new(:header => header, :words => ['serverinfo'])
	# m.header.sequence = 1
	# m.header.response = false
	# m.header.origin = :client
	# m.words << "asdf"
	m.write s
	loop {
	x = Frostbitten::Message.new
	x.read s
	puts x.words
	# if x.header.sequence 

	# end
	}
end