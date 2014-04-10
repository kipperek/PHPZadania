<?php
class Przedmioty{
	public $id;
	public $nazwa;
	public $id_parent;

	function __construct($i, $n, $i2){
		$this->id = $i;
		$this->nazwa = $n;
		$this->id_parent = $i2;
	}
}

class PrzedmiotyManager{
	
	private $przedmioty;
	private $base;
	function __construct(){
		$this->base = new DataBase();
		$this->przedmioty = $this->base->getPrzedmioty();
	}
	
	function searchShow($id, $sw){
		foreach ($this->przedmioty as &$p){
			if($p->id == $id){
				$sw[] = $p->id;
				if(!is_null($p->id_parent))
					$sw = $this->searchShow($p->id_parent,$sw);
					
				break;
			}
		}
		return $sw;
	}
	function showPrzedmioty(){
		$sw = array();
		if(isset($_GET['kat_id'])){
			$sw = $this->searchShow($_GET['kat_id'],$sw);
		}
		
		echo "<h2>Menu</h2><hr class='hrMenu'/>";
		
		if(isset($_SESSION['user_id']) && isset($_SESSION['user_login'])){
			$kat = "";
			if(isset($_GET['kat_id']))
				$kat= "&amp;kat_id=".$_GET['kat_id'];
				
			echo "- <a href='index.php?kat_form=2".$kat."'>Dodaj kategorię</a><br>";
			echo "- <a href='index.php?kat_form=1".$kat."'>Usuń kategorię</a><br>";
			echo "- <a href='index.php?kat_form=4".$kat."'>Modyfikuj kategorię</a><br>";
			echo "- <a href='index.php?kat_form=3".$kat."'>Przenieś kategorię</a><br><br>";
		}

		foreach ($this->przedmioty as &$p){
			
			if(is_null($p->id_parent))
				$this->printAndCheck($p->id,0,$sw);
		}
		
	}
	
	function printAndCheck($id, $sek, $sw){
		foreach ($this->przedmioty as &$p){
			if($p->id == $id){
				$bold = false;
				if(isset($_GET['kat_id']) && $_GET['kat_id'] == $id)
					$bold = true;
				
				for($i = 0; $i < $sek; $i++)
					echo '<span class="tab"></span>';        
				
				if($bold) echo '<b>';
				echo "<a href='index.php?kat_id=".$p->id."' class='drzewo'>".$p->nazwa."</a><br>";
				if($bold) echo '</b>';
				
				foreach ($sw as &$get)
					if($get == $id)
						foreach ($this->przedmioty as &$p2)
							if($p2->id_parent == $id)
								$this->printAndCheck($p2->id,$sek+1,$sw);
			}
		}
	}
	
	function showForm(){		
		if(isset($_GET['kat_form'])){
			if(isset($_SESSION['user_id']) && isset($_SESSION['user_login'])){
				$kat = "";
				if(isset($_GET['kat_id']))
					$kat= "?kat_id=".$_GET['kat_id'];
				
				switch($_GET['kat_form']){
					case 1: $this->printDeleteForm($kat);
						break;
					case 2: $this->printInsertForm($kat);
						break;
					case 3: $this->printMoveForm($kat);
						break;
					case 4: $this->printModifyForm($kat);
						break;
						
					default: break;
				}
			}
			else
				echo "Proszę się zalogować, aby zarządzać kategoriami.";
		
			echo "<br>";
		}
	}
	
	function printDeleteForm($kat){
		$send = false;
		$message = "";
		
		if(isset($_POST['delete_id'])){
			if($this->base->deleteKat($_POST['delete_id']) == 1){
				$send = true;
				echo "Pomyślnie usunięto kategorię - <a href='index.php".$kat."' >ok</a>";
			}
			else
				$message = "Wystąpił błąd przy usuwaniu kategorii";
		}
		
		if(!$send){
			echo "<h2>Usuwanie kategorii</h2><hr class='hrContent' />";
			echo "<span class='form_error' >".$message."</span>";
			echo "<form method='post' action='index.php".$kat;
			if($kat == "") echo "?";
			else echo "&amp;";
			echo "kat_form=1'>";
			echo "<table class='formTable'>";
			echo "<tr><td>Kategoria do usunięcia:</td><td><select name='delete_id'>";
			
			foreach ($this->przedmioty as &$p)
				echo "<option value='".$p->id."'>".$p->nazwa."</option>";
			
			echo "</select></td></tr></table>";
			echo "<input type='submit' value='Usuń' > <input type='reset' value='Resetuj' > <a href='index.php".$kat."' >Anuluj</a>";
			echo "</form>";
		}
	}
	
	function printModifyForm($kat){
		$send = false;
		$message = "";
		
		if(isset($_POST['modify_id']) && isset($_POST['modify_name'])){
			if($_POST['modify_name'] == "")
				$message = "Pole 'nowa nazwa' jest wymagane.";
			else if($this->base->modifyKat($_POST['modify_id'],$_POST['modify_name']) == 1){
				$send = true;
				echo "Pomyślnie zmodyfikowano kategorię - <a href='index.php".$kat."' >ok</a>";
			}
			else
				$message = "Wystąpił błąd przy modyfikowaniu kategorii.";
		}
		
		if(!$send){
			echo "<h2>Modyfikowanie kategorii</h2><hr class='hrContent' />";
			echo "<span class='form_error' >".$message."</span>";
			echo "<form method='post' action='index.php".$kat;
			if($kat == "") echo "?";
			else echo "&amp;";
			echo "kat_form=4'>";
			echo "<table class='formTable'>";
			echo "<tr><td>Modyfikowana kategoria:</td><td><select name='modify_id'>";
			
			foreach ($this->przedmioty as &$p)
				echo "<option value='".$p->id."'>".$p->nazwa."</option>";
			
			echo "</select></td></tr>";
			echo "<tr><td>Nowa nazwa<span class='gw'>*</span>:</td><td><input type='text' name='modify_name' ></td></tr></table><input type='submit' value='Zmień' > <input type='reset' value='Resetuj' >";
			echo " <a href='index.php".$kat."' >Anuluj</a></form>";
		}
	}
	function printMoveForm($kat){
		$send = false;
		$message = "";
		
		if(isset($_POST['move_to_id']) && isset($_POST['move_id'])){
			if($_POST['move_to_id'] == 0)
				$_POST['move_to_id'] = null;
				
			if($this->base->moveKat($_POST['move_id'],$_POST['move_to_id']) == 1){
				$send = true;
				echo "Pomyślnie przeniesiono kategorię - <a href='index.php".$kat."' >ok</a>";
			}
			else
				$message = "Wystąpił błąd przy przenoszeniu kategorii";
		}
		
		if(!$send){
			echo "<h2>Przenoszenie kategorii</h2><hr class='hrContent' />";
			echo "<span class='form_error' >".$message."</span>";
			echo "<form method='post' action='index.php".$kat;
			if($kat == "") echo "?";
			else echo "&amp;";
			echo "kat_form=3'>";
			echo "<table class='formTable'>";
			echo "<tr><td>Przenieś:</td><td><select name='move_id'>";
			
			foreach ($this->przedmioty as &$p)
				echo "<option value='".$p->id."'>".$p->nazwa."</option>";
			
			echo "</select></td></tr>";
			echo "<tr><td>Do:</td><td><select name='move_to_id'>";
			echo "<option value='0'>Korzeń</option>";
			foreach ($this->przedmioty as &$p)
				echo "<option value='".$p->id."'>".$p->nazwa."</option>";
			
			echo "</select></td></tr></table>";
			echo "<input type='submit' value='Przenieś' > <input type='reset' value='Resetuj' > <a href='index.php".$kat."' >Anuluj</a>";
			echo "</form>";
		}
	}
	function printInsertForm($kat){
		$send = false;
		$message = "";
		
			
		if(isset($_POST['insert_parent']) && isset($_POST['insert_name'])){
			if($_POST['insert_parent'] == 0)
				$_POST['insert_parent'] = null;
		
			if($_POST['insert_name'] == "")
				$message = "Pole 'nazwa' jest wymagane.";
			else if($this->base->insertKat($_POST['insert_parent'],$_POST['insert_name']) == 1){
				$send = true;
				echo "Pomyślnie dodano kategorię - <a href='index.php".$kat."' >ok</a>";
			}
			else
				$message = "Wystąpił błąd przy dodawaniu kategorii.";
		}
		
		
		if(!$send){
			echo "<h2>Dodawanie kategorii</h2><hr class='hrContent' />";
			echo "<span class='form_error' >".$message."</span>";
			echo "<form method='post' action='index.php".$kat;
			if($kat == "") echo "?";
			else echo "&amp;";
			echo "kat_form=2'>";
			echo "<table class='formTable'>";
			echo "<tr><td>Nadkategoria:</td><td><select name='insert_parent'>";
			
			echo "<option value='0'>Korzeń</option>";
			foreach ($this->przedmioty as &$p)
				echo "<option value='".$p->id."'>".$p->nazwa."</option>";
			
			echo "</select></td></tr>";
			echo "<tr><td>Nazwa<span class='gw'>*</span>:</td><td><input type='text' name='insert_name' ></td></tr></table><input type='submit' value='Dodaj' > <input type='reset' value='Resetuj' >";
			echo "  <a href='index.php".$kat."' >Anuluj</a></form>";
		}
	}
}
?>