import React, {useState} from 'react';
import ReactDOM from 'react-dom/client';
import './landing_page.css'
import {Button, AppBar, Container, Toolbar, Typography, ButtonGroup, Avatar, Dialog, DialogActions, DialogContent, DialogTitle} from '@mui/material';

/*
React Landing Page
Authors: Sreyansh Mamidi, Siddharth Tangri
Date: 12/6/2022
References: Material UI (https://mui.com/), https://marketing.illinois.edu/visual-identity/color
*/

/*
Goal: 
Add a map from dining hall to another map from day to an array of meal counts
Data for 6 weeks
Need to use integer division (divide by 6)
*/
const dining_meal_counts = {
    "ISR" : {
        "Sunday" : [[9, 10], [10, 14], [16, 20], [376, 3332, 5204]],
        "Monday" : [[7, 10], [10, 14], [16, 20], [1088, 6436, 6637]],
        "Tuesday" : [[7, 10], [10, 14], [16, 20], [1954, 9207, 8688]],
        "Wednesday" : [[7, 10], [10, 14], [16, 20], [1945, 9517, 8726]],
        "Thursday" : [[7, 10], [10, 14], [16, 20], [1969, 9481, 8642]],
        "Friday" : [[7, 10], [10, 14], [16, 20], [1913, 8629, 8629]],
        "Saturday" : [[9, 10], [10, 14], [16, 20], [830, 5664, 7076]],
    },
    "IKE" : {
        "Sunday" : [[9, 10], [10, 14], [16, 20], [411, 5662, 6514]],
        "Monday" : [[7, 10], [10, 15], [16, 20], [1362, 6317, 8019]],
        "Tuesday" : [[7, 10], [10, 15], [16, 20], [2369, 8427, 10897]],
        "Wednesday" : [[7, 10], [10, 15], [16, 20], [2363, 8036,10669]],
        "Thursday" : [[7, 10], [10, 15], [16, 20], [2349, 8239, 10038]],
        "Friday" : [[7, 10], [10, 15], [16, 20], [2326, 8313, 8313]],
        "Saturday" : [[9, 10], [10, 14], [16, 20], [942, 9327, 9866]],
    },
    "PAR" : {
        "Sunday" : [[-1, -1], [10, 15], [16, 20], [-1, 3127, 3156]],
        "Monday" : [[7, 10], [10, 15], [16, 20], [806, 2572, 4548]],
        "Tuesday" : [[7, 10], [10, 15], [16, 20], [1469, 3507, 5646]],
        "Wednesday" : [[7, 10], [10, 15], [16, 20], [1543, 2966, 5718]],
        "Thursday" : [[7, 10], [10, 15], [16, 20], [1420, 3613, 5585]],
        "Friday" : [[7, 10], [10, 15], [16, 20], [1575, 3634, 4621]],
        "Saturday" : [[-1, -1], [10, 15], [16, 20], [-1, 5082, 4535]],
    },
    "LAR" : {
        "Sunday" : [[-1, -1], [10, 14], [16, 19], [-1, 1086, 1101]],
        "Monday" : [[-1, -1], [10, 15], [16, 19], [-1, 1381, 1299]],
        "Tuesday" : [[-1, -1], [10, 15], [16, 19], [-1, 1805, 1942]],
        "Wednesday" : [[-1, -1], [10, 15], [16, 19], [-1, 1872, 1621]],
        "Thursday" : [[-1, -1], [10, 15], [16, 19], [-1, 1834, 1547]],
        "Friday" : [[-1, -1], [10, 15], [-1, -1], [-1, 1826, 1346]],
        "Saturday" : [[-1, -1], [10, 14], [16, 19], [-1, 1404, 1280]],
    }
}

export default function DiningDiego() {
    const [showPopUp, setShowPopUp] = useState(false)
    const [showInvalidPopUp, setShowInvalidPopUp] = useState(false)

    const [hallCapacity, setHallCapacity] = useState(-1)

    const intro_message = "Select a dining hall to get started.";
    var now = new Date();
    var date = now.toLocaleDateString();
    var time = now.toLocaleTimeString();
    var day = now.getDay();

    const day_arr = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
    const days_of_week = new Map();

    for (let i = 0; i < day_arr.length; ++i) {
        days_of_week.set(i, day_arr[i]);
    }

    let day_of_week = days_of_week.get(day);

    function findResult(hall, time, day) {
        // First need to truncate/modify the time
        var hour = ""
        for (let index = 0; index < time.length; ++index) {
            if (time.charAt(index) !== ':') {
                hour += time.charAt(index)
            } else {
                break
            }
        }
        const am_or_pm = time.substring(time.length - 2)
        var int_hour = parseInt(hour)
        if (am_or_pm === "PM") {
            int_hour += 12
        }

        const day_arr = dining_meal_counts[hall][day]

        // Initial value is -6, since -6/6 = -1
        var meal_count = -6
        var actually_open = 0
        for (let i = 0; i < 3; ++i) {
            if (int_hour >= day_arr[i][0] && int_hour < day_arr[i][1]) {
                meal_count = day_arr[3][i]
                actually_open = 1
                break
            }
        }

        // Can return multiple values in JavaScript by using an array
        return [Math.floor(meal_count / 6), actually_open]
    }

    function displayDiningInfo(hall, time, day) {
        const output = findResult(hall, time, day)
        setHallCapacity(output[0])

        if (hallCapacity === -1 && output[1] === 0) {
            setShowInvalidPopUp(true)
        } else {
            setShowPopUp(true)
        }
    }

    /**
    * Note: JSX expressions must have 1 parent element -> can't return multiple values
    */
    return (
    <>
        {/* Creates the navigation bar, displays the current date and time */}
        <AppBar position="static" style={{background: '#13294B'}}>
            <Container maxWidth="x1">
                <Toolbar disableGutters>
                    <Typography variant="h6" sx={{fontFamily: 'monospace', fontWeight: 500}}> <strong> Dining Diego at the University of Illinois </strong>  — </Typography>
                    &nbsp;
                    &nbsp;
                    <Typography variant="h6" sx={{fontFamily: 'monospace', fontWeight: 500}}> {day_of_week}   {date}           {time} </Typography>
                    &nbsp;
                    &nbsp;
                    &nbsp;
                    <Avatar alt="UIUC" src="https://marketing.illinois.edu/wp-content/uploads/2021/09/block-I-orange-background.png" sx={{width: 40, height: 40}}/>
                </Toolbar>
            </Container>
        </AppBar>
        {/* Provides insructions about how to use the web app */}
        <div>
            <h2 class="main-head" style={{color: '#FF5F05', fontFamily: 'monospace'}}> Welcome </h2>
            <div className='message' style={{fontFamily: 'sans-serif'}}> {intro_message} </div>
            <div className='page'>
                <br></br>
                <h2 class="main-head" style={{color: '#FF5F05', fontFamily: 'monospace'}}> Dining Halls </h2>
                <div className='message' style={{fontFamily: 'sans-serif'}}> The University of Illinois has <strong> four </strong> buffet-style dining options. </div>
                <br></br>
                {/* Displays the dining halls that the user can choose from */}
                <ButtonGroup variant="outlined" size="large">
                    <Button
                        variant="contained"
                        size="large"
                        style = {{backgroundColor: '#13294B', maxWidth: '150px', maxHeight: '100px', minWidth: '150px', minHeight: '100px'}}
                        onClick={() => displayDiningInfo("ISR", time, day_of_week)}
                        >
                        <span style={{fontSize: '20px'}}> ISR </span>
                    </Button>
                    &nbsp;
                    <Button
                        variant="contained"
                        size="large"
                        style = {{backgroundColor: '#13294B', maxWidth: '150px', maxHeight: '100px', minWidth: '150px', minHeight: '100px'}}
                        onClick={() => displayDiningInfo("IKE", time, day_of_week)}
                        >
                        <span style={{fontSize: '20px'}}> IKE </span>
                    </Button>
                    &nbsp;
                    <Button
                        variant="contained"
                        size="large"
                        style = {{backgroundColor: '#13294B', maxWidth: '150px', maxHeight: '100px', minWidth: '150px', minHeight: '100px'}}
                        onClick={() => displayDiningInfo("PAR", time, day_of_week)}
                        >
                        <span style={{fontSize: '20px'}}> PAR </span>
                    </Button>
                    &nbsp;
                    <Button
                        variant="contained"
                        size="large"
                        style = {{backgroundColor: '#13294B', maxWidth: '150px', maxHeight: '100px', minWidth: '150px', minHeight: '100px'}}
                        onClick={() => displayDiningInfo("LAR", time, day_of_week)}
                        >
                        <span style={{fontSize: '20px'}}> LAR </span>
                    </Button>
                </ButtonGroup>
                <Dialog open={showPopUp}>
                    <DialogTitle> 
                        <span style={{color: '#FF5F05', fontFamily: 'monospace', fontSize: 24}}> 
                        <strong> Success </strong> 
                        </span> 
                    </DialogTitle>
                    <DialogContent>
                        Hope you have a good meal! An average of <strong style={{color: '#13294B'}}> {hallCapacity} </strong> other individuals will also have this meal today, so plan wisely.
                    </DialogContent>
                    <DialogActions>
                        <Button
                            variant="contained"
                            style={{backgroundColor: '#13294B'}}
                            onClick={() => setShowPopUp(false)}
                        > OK 
                        </Button>
                    </DialogActions>
                </Dialog>
                <Dialog open={showInvalidPopUp}>
                    <DialogTitle> 
                        <span style={{color: '#FF5F05', fontFamily: 'monospace', fontSize: 24}}> 
                        <strong> Error </strong> 
                        </span> 
                    </DialogTitle>
                    <DialogContent>
                        Unfortunately, the dining hall that you've selected is closed or not offering this meal today.
                        <br></br>
                        <strong style={{color: '#13294B'}}> Please select a different dining hall. </strong>
                    </DialogContent>
                    <DialogActions>
                        <Button
                            variant="contained"
                            style={{backgroundColor: '#13294B'}}
                            onClick={() => setShowInvalidPopUp(false)}
                        > OK 
                        </Button>
                    </DialogActions>
                </Dialog>
            </div>
        </div>
    </>
    );       
}

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(<DiningDiego />);