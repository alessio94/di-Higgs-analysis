function toggleDiv(divName){
    var div = document.getElementById(divName);
    if(!div) return;
    if(div.style.display == "none") div.style.display = "block";
    else div.style.display = "none";
}