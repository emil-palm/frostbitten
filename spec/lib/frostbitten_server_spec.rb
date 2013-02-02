require 'spec_helper'
require 'frostbitten'
RCON_PORT = 11000
RCON_IP = "192.168.1.102"
RCON_PASSWORD = nil
RCON_PWD = ""
RCON_PWD = "@#{RCON_PASSWORD}" if RCON_PASSWORD 
RCON_URI = "fbrcon://#{RCON_PWD}#{RCON_IP}:#{RCON_PORT}"

describe Frostbitten::Server do
	it 'should parse a fbrcon uri' do
		server = Frostbitten::Server.new(RCON_URI)
		server.hostname.should == RCON_IP
		server.port.should == RCON_PORT  
		server.password.should == RCON_PASSWORD ? RCON_PASSWORD : nil
		server.sequence.should == 0

		server = Frostbitten::Server.new("fbrcon://moo@127.0.0.1")
		server.port.should == 47200
	end

	it 'should parse a fbrcon uri with options' do
		server = Frostbitten::Server.new(RCON_URI, {:sequence => 30})
		server.sequence.should == 30
	end

	it 'should return false on alive? since we havent connected' do
		server = Frostbitten::Server.new(RCON_URI)
		server.alive?.should == false
	end

	it 'should respond to connect' do
		server = Frostbitten::Server.new(RCON_URI)
		expect { server.connect }.to_not raise_error(NoMethodError)
	end

	it 'should return true on alive?' do
		server = Frostbitten::Server.new(RCON_URI)
		server.connect
		server.alive?.should == true
	end

	it 'should respond to close' do
		server = Frostbitten::Server.new(RCON_URI)
		server.connect
		expect { server.close }.to_not raise_error(NoMethodError)
	end
end