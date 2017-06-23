function updateDoorStatus(){

	$.getJSON("/updateDoorStatus/",function(data){
		
		if (data.status) {

			$('#door').prop('checked', true);
		
		}
		else { 

			$('#door').prop('checked', false);
		}
		
	});
};

function getDoorStatus(){

	$.getJSON("/getDoorStatus/",function(data){
		
		if (data.status) {

			$('#door').prop('checked', false);
		
		}
		else { 

			$('#door').prop('checked', true);
		}
		
	});
};

function getMotorStatus(){

	$.getJSON("/getMotorStatus/",function(data){
		
		if (data.status) {

			$('#motor').prop('checked', true);
		
		}
		else { 

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

	getDoorStatus();
	getMotorStatus();
	//alarm
	getBatteryStatus();

	setInterval(updateDoorStatus,500);
});