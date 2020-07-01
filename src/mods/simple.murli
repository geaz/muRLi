let h=0;
let s=0;
let v=0;

function init(){}

function update(delta){
    if(vol === 0 || freq === 0 || vol < lVol){
        if(v > 0.05){ v-=0.05; }
        else{ v=0; }
    } else {
        h=map(freq,0,maxF,180,0);
        s=1;
        v=map(vol,0,100,0,1);
    }
    let color=xhsv(h,s,v);
    for(let i=0; i<nLedC;i++){
        setLed(i, color);
    }
}

function getName() { return "easyMod"; }