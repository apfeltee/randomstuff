#!/usr/bin/env ruby

require "rubygems"
require "sequel"
require "securerandom"
require "logger"
require "ap"

def mdump(obj)
  return (obj.methods - Object.methods)
end

def mp(obj)
  ap mdump obj
end

=begin
def id_exists?(id)
  return (@db.where(:id => id).count > 0)
end
=end

def gen_id(len=20, attempts=0)
  # SecureRandom.hex returns a string twice the length given as
  # argument, so we halve the argument given to gen_id...
  # it's very silly, but it works, so don't judge me
  val = SecureRandom.hex(len / 2)
  #if not id_exists? val then
  #  if attempts == 100 then
  #    raise Error, "gen_id has looped for 100 times, something is broken!"
  #  end
  #  return gen_id(len, attempts+1)
  #end
  return val
end

# connect to a database
@db = Sequel.connect(
  :adapter => 'sqlite',
  #:database => 'foo.db',
  #:loggers => [Logger.new($stdout)]
)
#@db.sql_log_level = :debug

# create an items table
if not @db.table_exists? :items then
  @db.create_table :items do
    String      :id
    String      :name
    Float       :price
  end
end

# create a dataset from the items table
items = @db[:items]

# populate the table
items.insert(:id => gen_id, :name => 'abc', :price => rand * 100)
items.insert(:id => gen_id, :name => 'def', :price => rand * 100)
items.insert(:id => gen_id, :name => 'ghi', :price => rand * 100)

# print out the number of records
puts "Item count: #{items.count}"

# print out the average price
puts "The average price is: #{items.avg(:price)}"

items.each do |i|
  puts "id = #{i[:id]}, name = #{i[:name]}"
end

p items.where(:name => 'abc').count


