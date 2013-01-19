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

    hideLoadingAnimation: function () {
        var form = $(this),
            loadingBox = $('#loading-search');
        if (WheresMoBike.loadingAnimationComplete) {
            loadingBox.fadeOut();
            WheresMoBike.loadingAnimationComplete = false;
        } else {
            form.stop();
            form.hide();
            loadingBox.stop();
            loadingBox.hide();
        }
    },

    showSearchResults: function (results) {
        var resultsDiv = $('#search-results');
        var resultsUl = resultsDiv.children('ul');

        resultsUl.empty();
        results.forEach(function (result) {
            resultsUl.append(WheresMoBike.showSearchResult(result));
        });

        resultsDiv.fadeIn();
    },

    showSearchResult: function (result) {
        return '<li class="row">' +
            '<div class="span2">' +
              '<a target="_blank" href="' + result.url + '">' +
                '<img src="' + result.picture + '" />' +
              '</a>' +
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
                  WheresMoBike.hideLoadingAnimation)

        .delegate('#picture-select', 'click', function () {
            $('input[name=picture]').click();
        })
        .delegate('input[name=picture]', 'change', function () {
            var filename = $(this).val();
            filename = filename.replace(/^.+\\/, '');
            $('#picture-path').text(filename);
        });

})(jQuery);
