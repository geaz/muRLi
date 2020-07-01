let h=0;
let s=0;
let v=0;

function init(){
    setDelay(pLedC);
}

function update(delta){
    if(vol === 0 || freq === 0 || vol < lVol){
        if(v > 0.05){ v-=0.05; }
        else{ v=0; }
    } else {
        h=map(freq,0,maxF,180,0);
        s=1;
        v=map(vol,0,100,0,1);
    }

    for(let i=nLedC-1; i>0;i--){
        setLed(i, getLed(i-1));
    }
    setLed(0, xhsv(h,s,v));
}

function getName() { return "snakeMod"; }