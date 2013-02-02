require 'spec_helper'
require 'frostbitten'

describe Frostbitten do
  it 'should return correct version string' do
    Frostbitten::Version.string.should == "Frostbitten version #{Frostbitten::Version::STRING}"
  end
end