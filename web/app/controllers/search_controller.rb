class SearchController < ApplicationController
  def search
    @picture = params[:picture]
    respond_to do |format|
      format.html
      format.js
    end
  end
end
