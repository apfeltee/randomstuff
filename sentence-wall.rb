#!/usr/bin/env ruby

# prints a string as a block.
# usage: $0 "bullshit"
# yields:
#
#   b u l l s h i t
#   u l l s h i t b
#   l l s h i t b u
#   l s h i t b u l
#   s h i t b u l l
#   h i t b u l l s
#   i t b u l l s h
#   t b u l l s h i
#   b u l l s h i t
#

class SentenceWall
  def initialize(data, maxrows=nil)
    @data = data
    @dlength = @data.length
    # make a block using inputlen if maxrows is nil
    @maxrows = (maxrows == nil) ? @dlength : maxrows
    @elements = []
  end

  def iter(ibegin, iend)
    temp = @data[ibegin, iend] + @data
    row = temp[0, @dlength]
    @elements << row
  end

  def str(joinstr=" ")
    buf = []
    if @elements.length == 0 then
      ibegin = 0
      iend = @dlength
      # @maxrows.times fucks this up for some reason
      (0 .. @maxrows).each do
        iter(ibegin, iend)
        ibegin += 1
      end
    end
    @elements.each do |row|
      buf << row.join(joinstr)
    end
    buf.join("\n")
  end

  alias :to_s :str
end

chars = %w(E N T E R P R I S E P R O G R A M M I N G)
chars = (ARGV.length > 0) ? ARGV[0].split(//) : chars
puts SentenceWall.new(chars)


