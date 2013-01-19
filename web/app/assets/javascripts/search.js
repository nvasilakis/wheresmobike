$(document)
    .delegate('#search-form', 'ajax:before', function () {
        console.log('before');
        return true;
    })
    .delegate('#search-form', 'ajax:success', function () {
        console.log('success');
    })
    .delegate('#search-form', 'ajax:complete', function () {
        console.log('complete');
    });
