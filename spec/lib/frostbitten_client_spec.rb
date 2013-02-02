require 'spec_helper'
require 'frostbitten'

describe Frostbitten::Client do
	it { should respond_to(:send) }
	it { should respond_to(:server)}
	it { should respond_to(:server=)}

	it 'should update server upon server=' do
		server = double("Frostbitten::Server")
		client = Frostbitten::Client.new
		expect { client.server = server}.to change{client.server}.from(nil).to(server)
	end
end