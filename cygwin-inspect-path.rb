#!/usr/bin/ruby

require 'pp'

def get_ext(filename)
  ext = File.extname(filename)
  if ext == "" then
    return nil
  end
  ext.downcase!
  if ext[0] == '.' then
    return ext[1,ext.length]
  end
  return ext
end

def is_exefile(file)
  bad_extensions = %w(
    uninstall ps1xml
    dll  cpx  mib  tlb  ax   cpl  uce  oca
    ocx  nls  msc  xsl  tsp  sys  iec  rat
    dat  inf  acm  ime  nfo  txt  lex  rtf
    xml  drv  chs  cht  jpn  kor  tha  rs
    sep  ico  ini  xtr  h    mof  log  mif
    dmp  bin  prx  pdb
  )
  if File.file? file then
    base = File.basename(file)
    match = base.match(/\.([eE][xX][eE])$/)
    isdll = base.match(/\.([dD][lL][lL])$/)
    ext = get_ext(base)
    retval =
    {
      path: file,
      basename: base,
      regmatch: match,
      type: "unknown",
    }
    if bad_extensions.include?(ext) then
      return nil
    else
      if ext == "exe" then
        retval[:is_exefile] = true
        retval[:type] = "exefile"
      elsif File.executable?(file) then
        retval[:is_script] = true
        retval[:type] = "program"
      else
        return nil
      end
      return retval
    end
  end
  return nil
end

ENV["PATH"].split(/:/).each do |path|
  puts "EXE files in '#{path}':"
  Dir.entries(path).each do |file|
    info = is_exefile(File.join(path, file))
    if info then
      puts "  <#{info[:type]}> #{info[:path]}"
    end
  end
end
