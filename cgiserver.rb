#!/usr/bin/env ruby

require 'webrick'

class CGIHTTPServer
  def initialize(*args)
    wbargs = {}
    kwargs = args.shift
    kwargs.each do |key, value|
      case key
        when :port, :Port
          wbargs[:Port] = value
        when :docroot, :DocumentRoot
          wbargs[:DocumentRoot] = value
        when :bindaddr, :BindAddress
          wbargs[:BindAddress] = value
        when :timeout, :reqtimeout, :RequestTimeout
          wbargs[:RequestTimeout] = value
        when :callback, :reqcallback, :RequestCallback
          wbargs[:RequestCallback] = value
        when :docrootopts, :DocumentRootOptions
          wbargs[:DocumentRootOptions] = value
        when :log, :accesslog, :AccessLog
          wbargs[:AccessLog] = value
        else
          raise Exception, "unknown key '#{key}'"
      end
    end
    @server = WEBrick::HTTPServer.new(wbargs)
  end

  def on_signal(sig, &block)
    trap(sig) do
      block.call
    end
  end

  def start
    @server.start
  end

  def shutdown
    @server.shutdown
  end
end

srv = CGIHTTPServer.new({
  port: 8000,
  docroot: File.join(Dir.pwd, "."),
  docrootopts: {FancyIndexing: true,},
})
srv.on_signal "INT" do
  srv.shutdown
end

#p   WEBrick::Config::FileHandler
srv.start


