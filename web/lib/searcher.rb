require 'net/http'
require 'nokogiri'
require 'tempfile'

module Searcher

  def self.solr_url(url)
    @solr_url = url
  end

  def self.inpipe_name(name)
    @inpipe_name = name
  end

  def self.outpipe_name(name)
    @outpipe_name = name
  end

  def self.comparer_command(command)
    @comparer_command = command
  end

  def self.start
    return unless @comparer_command.present?
    `mkfifo #{@inpipe_name}` unless File.exists? @inpipe_name
    `mkfifo #{@outpipe_name}` unless File.exists? @outpipe_name
    @inpipe = File.open(@inpipe_name, 'r+')
    @outpipe = File.open(@outpipe_name, 'w+')

    pid = spawn(@comparer_command, :in => @outpipe_name, :out => @inpipe_name)
    @inpipe.gets # HELO
    puts "Started KNN pid=#{pid}"
  end

  def self.search_image(paths)
    return [] unless @comparer_command.present? && paths.present?

    res_file = "/tmp/WheresMoBikeSearch-#{SecureRandom.hex(16)}.xml"

    msg = "#{paths.size} #{res_file} #{paths.join ' '}"
    puts "Sending message to searcher: #{msg}"
    @outpipe.puts msg
    @outpipe.flush

    puts "Waiting for answer..."
    code = @inpipe.gets

    case code
    when /ACK/
      file = File.open(res_file)
      xml = Nokogiri::XML(file)
      file.close
      xml.css('postid').map do |id|
        # NB The name of this field is wrong, should've been url
        id.content
      end
    else
      puts "Image search failed: #{code}"
      []
    end
  end

  def self.postid(url)
    file = url.split('/').last
    file.split('.').first
  end

  def self.search(options={})
    solr_uri = URI(@solr_url)

    if options[:picture].present?
      urls = self.search_image [options[:picture]]
    else
      urls = []
    end

    q = options[:description].present? ?
        options[:description] + " AND " : ""

    q = q + "date:[#{options[:date].utc.iso8601} TO NOW] " +
      urls.map { |url| "postId:#{postid url}" }.join(' ')
    solr_params = {
      :q => q,
      :rows => 20,
      :indent => 'on'
    }
    solr_uri.query = URI.encode_www_form(solr_params)
    res = Net::HTTP.get_response(solr_uri)
    xml = Nokogiri::XML.parse(res.body)

    puts solr_uri
    puts res

    solr_results = xml.css('result doc').map do |result|
      title = result.at_css('str[name=title]')
      body = result.at_css('str[name=body]')
      picture = result.at_css('arr[name=imageURL] str')
      url = result.at_css('str[name=url]')

      {
        :title => title ? title.content : '',
        :body => body ? body.content : '',
        :picture => picture ? picture.content : nil,
        :url => url ? url.content : nil
      }
    end

  end

end
