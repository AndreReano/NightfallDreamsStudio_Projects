var canvas = document.getElementById("canvas");
var surface = canvas.getContext("2d");
var scorePara = document.getElementById("score");
var timePara = document.getElementById("time");
var shipBoom = document.getElementById("audioExplosion");
var alienBoom = document.getElementById("alienHit");
var missileSound = document.getElementById("missilePew");

const SCROLL = 5;
const ROWS = 6;
const COLS = 7;
const SIZE = 100;

canvas.width = COLS*SIZE;
canvas.height = ROWS*SIZE;

var leftPressed = false;
var rightPressed = false;
var upPressed = false;
var downPressed = false;
var spacePressed = false;

var points = 0;
var timer = 0;

var aliens = [];
var aIndex = 0;
var alienXplo;
var alienDeath = false;
var showA = true;


var map = [];
var player = {x: canvas.width/2-SIZE/2, y:SIZE*5, speed:10, dX:0, dY:0, image:null};

var missile = {x: -100, y:-100, speed: 15, image:null}; //creating missile object
var missileShoot = false;
var missile2;

var ship;
var rock;
var blank;
var shipXplo;

var easy=1;
var normal=2;
var hard=3;
var impossible = 4;

gameInit();

var intVal = setInterval(gameLoop, 33.34);


function gameInit()
{
    ship = new Image();
    ship.src = "../Covenant Attack(JS)/img/ship.png";
    player.image = ship;
    rock = new Image();
    rock.src = "../Covenant Attack(JS)/img/rock2.png";
    blank = new Image();
    blank.src = "../Covenant Attack(JS)/img/blank.png";
    missile.image = new Image();
    missile.image.src = "../Covenant Attack(JS)/img/missile.png";
    shipXplo = new Image();
    shipXplo.src = "../Covenant Attack(JS)/img/shipxplo.png";
    alienXplo = new Image();
    alienXplo.src = "../Covenant Attack(JS)/img/alienxplo.png";
    missile2 = new Image();
    missile2.src = "../Covenant Attack(JS)/img/missile.png";

    // Loop storing alien images into an enemy object
    for(var i = 0; i < 3; i++)
    {
        var enemy = {x:-SIZE, y:-SIZE, speed: SCROLL, image: null}; // creating enemy object
        enemy.image = new Image();

        if(i == 0)
        {
            enemy.image.src = "../Covenant Attack(JS)/img/alien1.png";
        }
        if(i == 1)
        {
            enemy.image.src = "../Covenant Attack(JS)/img/alien2.png";
        }
        if(i == 2)
        {
            enemy.image.src = "../Covenant Attack(JS)/img/alien3.png";
        }
       aliens.push(enemy); // pushing enemy object into an array called aliens
    }

    generateMap();
    window.addEventListener("keydown", KeyDown);
    window.addEventListener("keyup", KeyUp);
}

function generateMap()
{
    for (var row = 0; row < ROWS + 1; row++)
    {
        map[row] = [];
        for(var col = 0; col < COLS; col++)
        {
            var tempTile = { x:col*SIZE, y:row*SIZE, image:null, collision:false };
            tempTile.image = blank;
            map[row][col] = tempTile;
        }
    }
}

function KeyDown(event)
{
    switch(event.keyCode)
    {
        case 32: //Space bar
            if ( spacePressed === false)
                missileSound.play();
                spacePressed = true;
            break;
        case 37: // Left.
            if ( leftPressed === false )
                leftPressed = true;
            break;
        case 38: // Up.
            if ( upPressed === false )
                upPressed = true;
             break;
        case 39: // Right.
            if ( rightPressed === false )
                rightPressed = true;
            break;
        case 40: // Down.
            if ( downPressed === false )
                downPressed = true;
            break;
        default:
            console.log("Unhandled key.");
            break;
    }
}

function KeyUp(event)
{
    switch(event.keyCode)
    {
        case 32: //Space bar
            spacePressed = false;
            break;
        case 37: // Left.
            leftPressed = false;
            break;
        case 38: // Up.
            upPressed = false;
            break;
        case 39: // Right.
            rightPressed = false;
            break;
        case 40: // Down.
            downPressed = false;
            break;
        default:
            console.log("Unhandled key.");
            break;
    }
}

function gameLoop()
{
    movePlayer();
    scrollMap();
    collision();
    if(missileShoot == true)(moveMissile()); //calls moveMissile function when missileShoot is changed to true

    scrollAliens();
    render();
}

function movePlayer()
{
    if(leftPressed === true && player.x > -20)
        player.x -= player.speed;
        
    if(rightPressed === true && player.x < canvas.width - 80)
        player.x += player.speed;
        
    if(upPressed === true && player.y > 5)
        player.y -= player.speed;
        
    if(downPressed === true && player.y < canvas.height - SIZE)
        player.y +=player.speed;

    if(spacePressed == true && !missileShoot) //missile shoots when space is pressed
    {
        missile.image = missile2;
        missile.x = player.x;   // missile shoots from player's x and y coordinates
        missile.y = player.y;
        missileShoot = true;
    }

}

function moveMissile()
{
    missile.y -= missile.speed; //controls missile speed
    if(missile.y < -SIZE){missileShoot = false}

    for(var row = 0; row < map.length; row++)
    {
        for(var col = 0; col < map[0].length; col++)
        {
            if(map[row][col].collision) //Collision check for missile
            {
                if((missile.x + 25 >= map[row][col].x + 9 || missile.x + SIZE / 2 >= map[row][col].x + 9)
                    && (missile.x + 25 <= map[row][col].x + SIZE - 18)) // checks if withing X-range of rocks
                {
                    if ((missile.y + 9 >= map[row][col].y + 10 || missile.y + SIZE - 18 >= map[row][col].y + 9)
                        && (missile.y + 9 <= map[row][col].y + SIZE - 20)) // checks if overlapping Y-range
                    {
                        missile.image = shipXplo; // missile explosion animation upon collision of rock
                        setTimeout(clearFire, 70); // clears missile explosion image after 70ms
                    }

                }

            }
        }
    }


    if(aliens[aIndex].x) // Missile collision with alien
    {
        if((missile.x + 25 >= aliens[aIndex].x + 9 || missile.x + SIZE / 2 >= aliens[aIndex].x + 9)
            && (missile.x + 25 <= aliens[aIndex].x + SIZE - 18)) // checks if within x-range
        {
            if ((missile.y + 9 >= aliens[aIndex].y + 10 || missile.y + SIZE - 18 >= aliens[aIndex].y + 9)
                && (missile.y + 9 <= aliens[aIndex].y + SIZE - 20)) // checks if within y-range
            {
                alienDeath = true;
                points += 10;
                missile.image = alienXplo; // Alien explosion animation becomes true upon collision
                alienBoom.play();
                setTimeout(clearFire, 10); //clears alien explosion image after 10ms
            }

        }

    }
}

function clearFire() // function clearing the missile explosion image
{
    missile.image = shipXplo;
    missileShoot = false;
}

setInterval(clock, 1000); // 1-second increment timer
function clock()
{
    timer++;
}

function scrollMap()  //scrolls rocks on map
{
    for(var row = 0; row < map.length; row++)
    {
        for(var col = 0; col < map[0].length; col++)
        {
            map[row][col].y += SCROLL;
        }
    }
    
    if (map[map.length-1][map[0].length-1].y >= canvas.height )// check if the last element in the 2D array has gone off the screen
    {
        var counter = 0;
        var tempRow = []; // create a temporary row to replace the last row of the map that will be taken off
        map.pop(); // Get rid of the last row of the map as it has gone off the screen
        for ( var c = 0; c < map[0].length; c++) // Fill the temporary row with non collision objects
        {
            var tempTile = {x: c * SIZE, y: -100, image: null, collision:false};
            tempTile.image = blank;// change to empty space
            tempRow.push(tempTile); // add each new tile to the end of the array
        }
        
        while(counter<easy && timer<30) //if counter < 1 it's easy mode until timer hits 30-seconds
        {
          var xPos = Math.floor(Math.random()*COLS);
          var tempTile = {x:  xPos * SIZE, y: -100, image: null, collision:true};
          tempTile.image = rock;
          tempRow[xPos] = tempTile;
          counter++;
        }

        while(counter<normal && timer>29 && timer < 50) // counter = 1 when timer = 30, Normal mode until 50-sec mark
        {   //increasing amount of rocks generated

            var xPos = Math.floor(Math.random()*COLS);
            var tempTile = {x:  xPos * SIZE, y: -100, image: null, collision:true};
            tempTile.image = rock;
            tempRow[xPos] = tempTile;
            counter++;
        }

        while(counter<hard && timer>49 && timer<70)// counter = 2 when timer is >49, Hard mode enabled
        {   //increasing amount of rocks generated
            var xPos = Math.floor(Math.random()*COLS);
            var tempTile = {x:  xPos * SIZE, y: -100, image: null, collision:true};
            tempTile.image = rock;
            tempRow[xPos] = tempTile;
            counter++;
        }

        while(counter<impossible && timer>69)// counter = 2 when timer is >49, Hard mode enabled
        {   //increasing amount of rocks generated
            var xPos = Math.floor(Math.random()*COLS);
            var tempTile = {x:  xPos * SIZE, y: -100, image: null, collision:true};
            tempTile.image = rock;
            tempRow[xPos] = tempTile;
        }
        

        map.unshift(tempRow); // add the new row at the beginning of the map
    }
}

function scrollAliens() //Scrolls aliens from random top 3 rows and either left or right
{
    if(showA)
    {
        showA = false;
        var randPos = Math.floor(Math.random() * 2); //generating either 1 or 0
        var randYpos = Math.floor(Math.random()*3)*SIZE; //generating random row position
        aliens[aIndex].y = randYpos;
        if (randPos == 0)
        {
            aliens[aIndex].x = -SIZE; //puts alien outside the left of canvas if rand = 0
            if(aliens[aIndex].speed < 0)    //moves alien from right to left
            {
                aliens[aIndex].speed *= -1;
            }
        }
        else
        {
            aliens[aIndex].x = canvas.width + SIZE; //puts alien outside the right of canvas if rand = 1
            if (aliens[aIndex].speed > 0)
            {
                aliens[aIndex].speed *= -1; //moves alien from left to right
            }
        }
    }
    aliens[aIndex].x += aliens[aIndex].speed;

    if((aliens[aIndex].speed > 0 && aliens[aIndex].x > canvas.width+SIZE)
        || (aliens[aIndex].speed<0 && aliens[aIndex].x < -SIZE) || alienDeath)
    {   //when alien is out of the canvas, image is shifted out, and pushed back into array aliens
        aliens[aIndex].x = -SIZE;
        var tempA = aliens.shift();
        aliens.push(tempA);
        showA = true;
        alienDeath = false;
    }
}

function collision() // player collision with asteroids function
{
    for(var row = 0; row < map.length; row++)
    {
        for( var col = 0; col < map[0].length; col++)
        {
            if (map[row][col].collision == true)
            {
                if ((player.x + 25 >= map[row][col].x + 9 || player.x + SIZE / 2 >= map[row][col].x + 9)
                    && (player.x + 25 <= map[row][col].x + SIZE - 18)) //checks if withing x-range of rocks
                {

                    if ((player.y + 9 >= map[row][col].y + 10 || player.y + SIZE - 18 >= map[row][col].y + 9)
                        && (player.y + 9 <= map[row][col].y + SIZE - 20)) //checks if within y-range of rocks
                    {
                        player.image = shipXplo; //if statements are satisfied explosion image shows
                        player.x -= 20; // makes the image appear more central to the ships explosion position
                        shipBoom.play();
                        setTimeout(endGame,50); //calls death function
                    }
                }
            }
        }
    }
}

function endGame() //death log
{
    clearInterval(intVal);
    alert("You have crashed... Earth will perish because of you're bad at games. RIP humanity.");
}


function render() {
    surface.clearRect(0, 0, canvas.width, canvas.height); // x, y, w, h

    // Render map...
    for (var row = 0; row < map.length; row++)
    {
        for (var col = 0; col < map[0].length; col++)
        {
            if (map[row][col].image != null)
            {
                surface.drawImage(map[row][col].image, map[row][col].x, map[row][col].y);
            }

        }
    }


    //Render alien...
    surface.drawImage(aliens[aIndex].image, aliens[aIndex].x, aliens[aIndex].y);


    //Render missile..
    if (missileShoot) //Missile will draw on screen when missileShoot becomes true
    {
        surface.drawImage(missile.image, missile.x, missile.y);
    }

    surface.fillText(scorePara.innerHTML = "Score: " + points, 0,0); //Score text
    surface.fillText(timePara.innerHTML = "Time: " + timer, 0,0);   //Timer text


    // Render player...
    surface.drawImage(player.image, player.x, player.y);
}




