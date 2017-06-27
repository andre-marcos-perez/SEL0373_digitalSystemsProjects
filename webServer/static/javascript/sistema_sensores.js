function updateSensorStatus(){

	$.getJSON("/updateSensorStatus/",function(data){
		
		if (data.topic == 'home/hallSensor'){
			if (data.status == true) {
				$('#door').prop('checked', true);
			}
			else if(data.status == false){ 
				$('#door').prop('checked', false);
			}
		}
		if (data.topic == 'home/battery'){
			$('#battery').text(data.status);
		}
	});
};

function getDoorStatus(){

	$.getJSON("/getDoorStatus/",function(data){
		
		if (data.status == true) {

			$('#door').prop('checked', false);
		
		}
		else if(data.status == false){ 

			$('#door').prop('checked', true);
		}
		
	});
};

function getMotorStatus(){

	$.getJSON("/getMotorStatus/",function(data){
		
		if (data.status == true) {

			$('#motor').prop('checked', true);
		
		}
		else if(data.status == false){ 

			$('#motor').prop('checked', false);
		}
		
	});
};

function getBatteryStatus(){
			
	$.getJSON("/getBatteryStatus/",function(data){
	
		$('#battery').text(data.status);
	}); 
};



$(document).ready(function(){

		
	//getDoorStatus();
	//getMotorStatus();
	//alarm
	//getBatteryStatus();

	setInterval(updateSensorStatus,500);
});
