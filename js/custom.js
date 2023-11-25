(function(window, document) {
    var elements = document.querySelectorAll('a.header > code');
    elements.forEach(function(x) {
        x.parentElement.className += ' rx-api';
    });
})(window, window.document);
