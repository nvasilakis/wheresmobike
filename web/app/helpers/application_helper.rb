module ApplicationHelper

  def navigation(name, address)
    style = if name == @current_tab
              'class="active"'
            else
              'class=""'
            end

    raw "<li #{style}><a href=\"#{address}\">#{name.to_s.capitalize}</a></li>"
  end

  def calendar_date(date)
    "#{"%02d" % date.month}/#{"%02d" % date.day}/#{date.year}"
  end

end
