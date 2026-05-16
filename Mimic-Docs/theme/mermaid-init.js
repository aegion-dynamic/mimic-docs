document.addEventListener("DOMContentLoaded", function() {
    var blocks = document.querySelectorAll("pre code.language-mermaid");
    blocks.forEach(function(block) {
        var pre = block.parentNode;
        var div = document.createElement("div");
        div.className = "mermaid";
        div.textContent = block.textContent;
        pre.parentNode.replaceChild(div, pre);
    });
    mermaid.initialize({ startOnLoad: true });
});
