var ex_dzc;
ex_dzc = {};

if (!ex_dzc.test)
  ex_dzc.test = {};
(function() {

  ex_dzc.test.myfunction = function() {
    native function MyFunction();
    return MyFunction();
  };
  
  ex_dzc.test.__defineGetter__('myparam', function() {
    
    native function GetMyParam();
    return GetMyParam();
  });
  
  ex_dzc.test.__defineSetter__('myparam', function(b) {
    native function SetMyParam();
    if(b) SetMyParam(b);
  });

  }
)();