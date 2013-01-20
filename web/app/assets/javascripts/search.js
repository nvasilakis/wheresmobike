var WheresMoBike = {

    loadingAnimationComplete: false,

    showLoadingAnimation: function () {
        var form = $(this),
            loadingBox = $('#loading-search');

        form.fadeOut(400, function () {
            loadingBox.fadeIn(400, function () {
                WheresMoBike.loadingAnimationComplete = true;
            });
        });
    },

    hideSearchForm: function () {
        var form = $(this),
            search = $('#search'),
            loadingBox = $('#loading-search');
        if (WheresMoBike.loadingAnimationComplete) {
            loadingBox.fadeOut();
            search.hide();
            WheresMoBike.loadingAnimationComplete = false;
        } else {
            form.stop();
            search.hide();
            loadingBox.stop();
            loadingBox.hide();
        }
    },

    showSearchResults: function (results) {
        var resultsDiv = $('#search-results');
        var resultsUl = resultsDiv.children('ul');

        resultsUl.empty();

        if (results.length == 0) {
            resultsDiv.children('.no-results').show();
        } else {
            resultsDiv.children('.no-results').hide();
            results.forEach(function (result) {
                resultsUl.append(WheresMoBike.showSearchResult(result));
            });
        }

        resultsDiv.fadeIn();
    },

    showSearchResult: function (result) {
        return '<li class="row">' +
            '<div class="span2">' +
              (result.picture ?
                '<a target="_blank" href="' + result.url + '">' +
                   '<img src="' + result.picture + '" />' +
                '</a>' :
                '<a target="_blank" class="no-image" ' +
                   'href="' + result.url + '">' +
                   'No Image Available' +
                '</a>') +
            '</div>' +
            '<div class="span10">' +
              '<div class="title">' +
                '<a target="_blank" href="' + result.url + '">' + result.title + '</a>' +
              '</div>' +
              '<span class="body">' + result.body + '</span>' +
            '</div>' +
            '</li>';
    }

};

(function($) {

    $(document)
        .delegate('#search-form', 'ajax:before',
                  WheresMoBike.showLoadingAnimation)
        .delegate('#search-form', 'ajax:complete',
                  WheresMoBike.hideSearchForm)

        .delegate('#picture-select', 'click', function () {
            $('input[name=picture]').click();
        })
        .delegate('input[name=picture]', 'change', function () {
            var filename = $(this).val();
            filename = filename.replace(/^.+\\/, '');
            $('#picture-path').text(filename);
        });
    $(function () {
        $('.field label a').tooltip({
            placement: 'right',
            title: function () {
                var field = $(this).closest('.field');
                return field.children('.field-description').text();
            }
        });
          //$(document).ready(function(){
            $('#show-one').fadeIn(1000, function(){
              $('#show-two').fadeIn(1000, function(){
                $('#show-three').fadeIn(1000, function(){
                  WheresMoBike.glDemoConcept();
                });
              });
            });
          //});
        $('.field input[name=date]').datepicker();

    });

})(jQuery);
