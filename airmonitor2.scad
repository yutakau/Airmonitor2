/* cap_cpuplate */

// 2018/6/5  15:35   usui
t=1.2;
h=52;
w=70;

//base-plate
module baseplate(h,w,t) {
    difference() {
        cube([h,w,t]);
        // LCD hole
        translate([8,14,0])      cube([15,20,20]);
    }
    

    //wall
    wt=t+20;
    translate([0,0,0])  cube([t,w,wt]);


    translate([h-t,0,0])  cube([t,w,wt]);
    
    translate([0,w-t,0]) 
        difference() {
            cube([h,t,wt]);
            translate([19,-2,7]) cube([14,12,9]); //USB
        }
    
    translate([0,0,0])  cube([h,t,wt]);


    //table
    //translate([h/2-4,w-t-5,0]) cube([10,5,3]);
};



//post
baseplate(h,w,t);

