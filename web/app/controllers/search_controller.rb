require 'searcher'

class SearchController < ApplicationController
  def search
    @picture = params[:picture]
    if @picture.present?
      old_name = @picture.tempfile.path
      new_dir = "#{Rails.root}/public/uploaded"
      new_name = "#{SecureRandom.hex 16}#{@picture.original_filename}"
      @picture = "#{new_dir}/#{new_name}"
      FileUtils.cp(old_name, @picture)
    end

    @description = params[:description]
    @date = Date.strptime(params[:date], '%m/%d/%Y').to_time

    @response = {
      :search => {},
      :results => Searcher.search(:picture => @picture,
                                  :description => @description,
                                  :date => @date)
    }

    if @picture.present?
      @response[:search][:picture] =
        "/uploaded/#{File.basename @picture}"
    end

    # uncomment to test waiting screen
    sleep 5

    respond_to do |format|
      format.html
      format.js
    end
  end
end
