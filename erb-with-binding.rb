
require 'erb'
require 'ostruct'

class RTemplate
    def initialize(str=nil)
        if str != nil
            create_erb_instance(str)
        end
    end

    def parsefile(path)
        create_erb_instance(IO.read(path))
        return self
    end

    def result(env={})
        namespace = OpenStruct.new(env)
        return @erb.result(namespace.instance_eval{binding})
    end

    def erb
        return @erb
    end

    def create_erb_instance(template_str)
        @erb = ERB.new(template_str)
        return self
    end

    private :create_erb_instance
end

foo = "bar"
rt = RTemplate.new(%q{Name: <%= name %> foo = <%= "%s" % foo.to_s %>})
puts rt.erb
puts(rt.result({"name" => "Joan Maragall"}))
