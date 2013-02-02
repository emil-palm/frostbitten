require 'spec_helper'
require 'frostbitten'

describe Frostbitten do
  it 'should return correct version string' do
    Frostbitten.version_string.should == "Frostbitten version #{Sudoku::VERSION}"
  end
end