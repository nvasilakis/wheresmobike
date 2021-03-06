require 'searcher'

class SearchController < ApplicationController
  def search
    @picture = params[:picture]
    if @picture.present?
      old_name = @picture.tempfile.path
      new_dir = "#{Rails.root}/public/uploaded"
      new_name = "#{SecureRandom.hex 16}#{@picture.original_filename}"
      @picture = "#{new_dir}/#{new_name}"
      @picture_proc = "#{new_dir}/proc#{new_name}"
      FileUtils.cp(old_name, @picture)
    end

    @description = params[:description]
    @date = Date.strptime(params[:date], '%m/%d/%Y').to_time

    @response = {
      :search => {
        :description => @description,
        :date => @date
      },
      :results => Searcher.search(:picture => @picture,
                                  :picture_proc => @picture_proc,
                                  :description => @description,
                                  :date => @date)
    }

    if @picture.present?
      @response[:search][:picture] =
        "/uploaded/#{File.basename @picture}"
      @response[:search][:picture_proc] =
        "/uploaded/#{File.basename @picture_proc}"
    end

    # uncomment to test waiting screen
    sleep 5

    respond_to do |format|
      format.html
      format.js
    end
  end
end
