class SearchController < ApplicationController
  def search
    @picture = params[:picture]
    respond_to do |format|
      format.html
    end
  end
end
