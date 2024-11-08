if (!!window.EventSource) {
    var source = new EventSource('/');
    source.onmessage = function(e) {
      var bumper = e.data[1]
      var cliff = e.data[3];
      var drop = e.data[5];


      // finish the code to handle the bumper status
        if (bumper=="0")
          {
            document.getElementById("bumper_status_button").value = "OFF";
          }
        if (bumper=="1")
        {
          document.getElementById("bumper_status_button").value = "Right";
        }
        if (bumper=="2")
        {
          document.getElementById("bumper_status_button").value = "Center";
        }
        if (bumper=="3")
        {
          document.getElementById("bumper_status_button").value = "Right & Center";
        }
        if (bumper=="4")
        {
          document.getElementById("bumper_status_button").value = "Left";
        }
        if (bumper=="5")
        {
          document.getElementById("bumper_status_button").value = "Left & Right";
        }
        if (bumper=="6")
        {
          document.getElementById("bumper_status_button").value = "Left & Center";
        }
        if (bumper=="7")
        {
          document.getElementById("bumper_status_button").value = "Left, Right, & Center";
        }
        
         
        
        // finish the code to handle the wheel drop status 
      if (drop=="0")
      {
        document.getElementById("wheel_drop_status_button").value = "OFF";
      }
      if (drop=="1")
      {
        document.getElementById("wheel_drop_status_button").value = "Right";
      }
      if (drop=="2")
      {
        document.getElementById("wheel_drop_status_button").value = "Left";
      }
      if (drop=="3")
      {
        document.getElementById("wheel_drop_status_button").value = "Left & Right";
      }
      

      // finish the code to handle cliff status 
      if (cliff=="0")
      {
        document.getElementById("cliff_status_button").value = "OFF";
      }
      if (cliff=="1")
      {
        document.getElementById("cliff_status_button").value = "Right";
      }
      if (cliff=="2")
      {
        document.getElementById("cliff_status_button").value = "Center";
      }
      if (cliff=="3")
      {
        document.getElementById("cliff_status_button").value = "Right & Center";
      }
      if (cliff=="4")
      {
        document.getElementById("cliff_status_button").value = "Left";
      }
      if (cliff=="5")
      {
        document.getElementById("cliff_status_button").value = "Left & Right";
      }
      if (cliff=="6")
      {
        document.getElementById("cliff_status_button").value = "Left & Center";
      }
      if (cliff=="7")
      {
        document.getElementById("cliff_status_button").value = "Left, Right, & Center";
      }
      


    }
  }