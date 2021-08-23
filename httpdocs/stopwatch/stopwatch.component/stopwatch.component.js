angular.module("stopwatch").
    component("stopwatch", {
        templateUrl: "./stopwatch.component/stopwatch.html",
        controller: function StopwatchController($scope, $timeout) { // timeout injected 

            $scope.isRunning = false;

            $scope.secondTitle = "00";
            $scope.minuteTitle = "00";

            var second = 0;
            var minute = 0;

            $scope.toggleStopwatch = function () {
                if ($scope.isRunning == false) {
                    $scope.isRunning = true;
                    run();
                }
                else {
                    $scope.isRunning = false;
                }
            }

            $scope.resetStopwatch = function () {
                minute = 0;
                second = 0;
                setStopwatchValue(second, minute);
            
                // If we want to reset while the stopwatch is running. Stop the stopwatch.
                if ($scope.isRunning) { 
                    $scope.toggleStopwatch();
                }
            }

            function run() {

                if ($scope.isRunning) {
    
                    second ++;
                    if (second == 60) {
                        minute ++;
                        second = 0;
                    }
    
                    setStopwatchValue(second,minute);
                        
                    $timeout(function () {
                        run();
                        }, 1000);
                }
            }
    
            function setStopwatchValue(second, minute) {
                if (second < 10 ) {
                    $scope.secondTitle = "0"+second;
                }
                else{
                    $scope.secondTitle = second;
                }
            
                if(minute < 10 ){
                    $scope.minuteTitle = "0"+minute;
                }

                else{
                    $scope.minuteTitle = minute;
                }
            }  
        }
    });