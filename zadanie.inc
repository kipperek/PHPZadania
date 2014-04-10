<?php
	class Zad{
		public $id;
		public $kat;
		public $name;
		public $link;
		public $rate;
		
		function __construct($id, $kat, $name, $link, $rate){
			$this->id = $id;
			$this->kat = $kat;
			$this->name = $name;
			$this->link = $link;
			$this->rate = $rate;
		}
	}
	
	class ZadaniaManager{
		private $data = array();
		private $kategoria;
		private $base;
		
		function __construct(){
			$this->base = new DataBase();
			if(isset($_GET['kat_id'])){
				$this->data = $this->base->getZadania($_GET['kat_id']);
				$this->kategoria = $this->base->getKategoria($_GET['kat_id']);
			}
		}
		
		function printTable(){
			
			$this->formManager();
			
			if(!isset($_GET['kat_id']))
				echo "<h2>Wybierz kategorie aby zobaczyć zadania.</h2>";
			else{
				echo "<h2>".$this->kategoria."</h2><hr class='hrContent' />";
				if(count($this->data)==0)
					echo "Brak zadań dla tej kategorii.";
			}
			echo "<table class='table'>";
					
			if(count($this->data)!=0){
				echo "<tr><th><b>Zadanie</b></th><th><b>Trudność</b></th>";
				if(isset($_SESSION['user_id']) && isset($_SESSION['user_login']))
					echo "<th>Akcje</th>";
				echo "</tr>";
			}
			
			foreach($this->data as &$row)
			{
				echo '<tr><td><a href="zadania/'.$row->link.'" target="_blank">'.$row->name."</a></td><td>";
				for($i=0;$i<$row->rate;$i++)
					echo "<img src='graph/g.png' alt='gwiazdka'/>";
				echo "</td>";
				
				if(isset($_SESSION['user_id']) && isset($_SESSION['user_login'])){
					echo "<td style='text-align: left;'><a href='index.php?delete_zad_id=";
					echo $row->id;
					echo '&amp;kat_id='.$_GET['kat_id'];
					echo "'><img src='graph/u.png' alt='Usun' /> Usuń</a>";
					
					echo "<br><a href='index.php?modify_zad_id=";
					echo $row->id;
					echo '&amp;kat_id='.$_GET['kat_id'];
					echo "'><img src='graph/m.png' alt='Modyfikuj'/> Modyfikuj</a></td>";
				}
				echo '</tr>';
			}
				
			echo "</table>";
			if(isset($_GET['kat_id']) && isset($_SESSION['user_id']) && isset($_SESSION['user_login'])){
				echo "<br><a href='index.php?add_zad=true";
				echo "&amp;kat_id=".$_GET['kat_id'];
				echo "'><img src='graph/d.png' alt='Dodaj'/> Dodaj zadanie</a>";
			}
		}
		
		function formManager(){
			$error = "Proszę się zalogować, aby zarządzać zadaniami.";
			if(isset($_GET['add_zad']) && isset($_GET['kat_id']) && $_GET['add_zad'] == "true"){
				if(isset($_SESSION['user_id']) && isset($_SESSION['user_login']))
					$this->insertForm();
				else
					echo $error;
			
			}else if(isset($_GET['delete_zad_id']) && isset($_GET['kat_id'])){
				if(isset($_SESSION['user_id']) && isset($_SESSION['user_login']))
					$this->deleteForm();
				else
					echo $error;
			
			}else if(isset($_GET['modify_zad_id']) && isset($_GET['kat_id'])){
				if(isset($_SESSION['user_id']) && isset($_SESSION['user_login']))
					$this->modifyForm();
				else
					echo $error;
			}
		}
		
		function deleteForm(){
			if(isset($_GET['delete']) && $_GET['delete'] == "true"){
				if($this->base->deleteZadanie($_GET['delete_zad_id']) > 0){
					echo "Usunięto zadanie o id ".$_GET['delete_zad_id']." - <a href='index.php?kat_id=".$_GET['kat_id']."' >ok</a>";
					$this->data = $this->base->getZadania($_GET['kat_id']);
				}
				else
					echo "Brak zadania o id ".$_GET['delete_zad_id']." - <a href='index.php?kat_id=".$_GET['kat_id']."' >ok</a>";
			}else{
				$zadanie_b = false;
				$zadanie_s;
				
				foreach($this->data as &$row)
				{
					if($row->id == $_GET['delete_zad_id']){
						$zadanie_b = true;
						$zadanie_s = $row->name;
						break;
					}
				}
				
				if($zadanie_b){
					echo "<h2>Potwierdź usunięcie zadania</h2><hr class='hrContent' />";
					echo "Czy na pewno usunąć zadanie <b>".$zadanie_s."</b> <a href='index.php?kat_id=".$_GET['kat_id']."&amp;delete_zad_id=".$_GET['delete_zad_id']."&amp;delete=true' >TAK</a>";
					echo " -  <a href='index.php?kat_id=".$_GET['kat_id']."'>NIE</a><br>";
				}
				else
					echo "Wybrane zadanie do usunięcia, nie istnieje";
			}
		}
		
		function insertForm(){
			$send = false;
			$message = "";
			$file_name;
			
			if(isset($_POST['new_zad_kat']) && isset($_POST['new_zad_trudnosc'])){
				$errors = false;
				
				if(isset($_POST['new_zad_name']) && is_uploaded_file($_FILES['new_zad_plik']['tmp_name']) && $_POST['new_zad_name'] != ""){
					if($_FILES['new_zad_plik']['type'] != 'application/pdf'){
						$errors = true;
						$message = 'Przesyłana treść zadania musi być plikiem PDF.';
					}
					else{
						$file_name = $_FILES['new_zad_plik']['name'];
						$file_name = str_replace(array("-",'"',"'","/","\\"),"",$file_name);
						$count = $this->base->checkPDF($file_name);
						if($count > 0)
							$file_name = $count.$file_name;
						move_uploaded_file($_FILES['new_zad_plik']['tmp_name'], "zadania/".$file_name);
					}
				}
				else{
					$errors = true;
					$message = 'Nie wszystkie pola są wypełnione.';
				}
				
				if(!$errors){ 
					if($this->base->insertZadanie($_POST['new_zad_name'], $_POST['new_zad_kat'], $file_name, $_POST['new_zad_trudnosc']) == 1){
						echo "Pomyślnie dodano zadanie - <a href='index.php?kat_id=".$_GET['kat_id']."' >ok</a>";
						$this->data = $this->base->getZadania($_GET['kat_id']);
						$send = true;
					}else
						$messsage = "Wystąpił problem, nie dodano zadania.";
					
				}
			}
			if(!$send){
				echo "<h2>Dodaj nowe zadanie</h2><hr class='hrContent' />";
				echo "<span class='form_error' >".$message."</span>";
				echo "<form method='post' action='index.php?add_zad=true&amp;kat_id=".$_GET['kat_id']."' ENCTYPE='multipart/form-data'><table class='formTable'>";
				echo "<tr><td>Nazwa<span class='gw'>*</span>:</td><td> <input type='text' name='new_zad_name' ></td></tr>";
				
				echo "<tr><td>Kategoria<span class='gw'>*</span>:</td><td><select name='new_zad_kat'>";
				$kategorie = $this->base->getPrzedmioty();
				foreach($kategorie as $kat){
					echo "<option value='".$kat->id."' ";
					if($_GET['kat_id'] == $kat->id)
						echo "selected='selected'";
					echo ">".$kat->nazwa."</option>";
				}			
				echo "</select></td></tr>";
				echo "<tr><td>Trudność<span class='gw'>*</span>:<br><span class='formDescription'>(1 - bardzo łatwe, 5 - bardzo trudne)</span></td><td>1.<input type='radio' name='new_zad_trudnosc' value='1' checked>";
				echo " 2.<input type='radio' name='new_zad_trudnosc' value='2'>  3.<input type='radio' name='new_zad_trudnosc' value='3'>";
				echo " 4.<input type='radio' name='new_zad_trudnosc' value='4'>  5.<input type='radio' name='new_zad_trudnosc' value='5'></td></tr>";
				echo "<tr><td>Plik z treścią zadania<span class='gw'>*</span>:<br><span class='formDescription'>Wymagane rozszeżenie pdf</span></td><td><input name='new_zad_plik' type='file'></td></tr>";	
				echo "</table><input type='submit' value='Dodaj' > <input type='reset' value='Resetuj' > <a href='index.php?kat_id=".$_GET['kat_id']."' >Anuluj</a></form><br>";
			}
		}
		
		function modifyForm(){
			$zadanie = $this->base->getZadanie($_GET['modify_zad_id']);
			
			if(!is_null($zadanie)){ 
				$send = false;
				$message = "";
				
				if(isset($_POST['mod_zad_kat']) && isset($_POST['mod_zad_trudnosc'])){
					$errors = false;
					$file_name = $zadanie->link;
					if(!isset($_POST['mod_zad_name']) || $_POST['mod_zad_name'] == ""){
						$errors = true;
						$message = "Wartość pola 'Nazwa' jest wymagana";
					}else if(is_uploaded_file($_FILES['mod_zad_plik']['tmp_name'])){
						if($_FILES['mod_zad_plik']['type'] != 'application/pdf'){
							$errors = true;
							$message = 'Przesyłana treść zadania musi być plikiem PDF.';
						}else{
							$file_name = $_FILES['mod_zad_plik']['name'];
							$file_name = str_replace(array("-",'"',"'","/","\\"),"",$file_name);
							$count = $this->base->checkPDF($file_name);
							if($count > 0)
								$file_name = $count.$file_name;
							move_uploaded_file($_FILES['mod_zad_plik']['tmp_name'], "zadania/".$file_name);
						}
					}
				
					if(!$errors){
						if($this->base->updateZadanie($zadanie->id, $_POST['mod_zad_name'],$_POST['mod_zad_kat'], $_POST['mod_zad_trudnosc'], $file_name) == 1){
							echo "Pomyślnie zmodyfikowano zadanie - <a href='index.php?kat_id=".$_GET['kat_id']."' >ok</a>";
							$send = true;
							$this->data = $this->base->getZadania($_GET['kat_id']);
						}
						else{
							$message = "Wystąpił błąd przy modyfikacji zadania, prawdopodobnie nie zostały zmienione żadne dane.";
						}
					}
				}
				
				if(!$send){
					echo "<h2>Modyfikuj '".$zadanie->name."'</h2><hr class='hrContent' />";
					echo "<span class='form_error' >".$message."</span>";
					echo "<form method='post' action='index.php?kat_id=".$_GET['kat_id']."&amp;modify_zad_id=".$zadanie->id."' ENCTYPE='multipart/form-data'><table class='formTable'>";
					echo "<tr><td>Nazwa<span class='gw'>*</span>:</td><td> <input type='text' name='mod_zad_name' value='".$zadanie->name."'></td></tr>";
					
					echo "<tr><td>Kategoria<span class='gw'>*</span>:</td><td><select name='mod_zad_kat'>";
					$kategorie = $this->base->getPrzedmioty();
					foreach($kategorie as $kat){
						echo "<option value='".$kat->id."' ";
						if($zadanie->kat == $kat->id)
							echo "selected='selected'";
						echo ">".$kat->nazwa."</option>";
					}			
					echo "</select></td></tr>";
					echo "<tr><td>Trudność<span class='gw'>*</span>:<br><span class='formDescription'>(1 - bardzo łatwe, 5 - bardzo trudne)</span></td><td>";
					for($i=1; $i <= 5; $i++){
						echo $i.".<input type='radio' name='mod_zad_trudnosc' value='".$i."' ";
						if($i == $zadanie->rate) echo "checked";
						echo "> ";
					}
					echo "</td></tr>";
					echo "<tr><td>Plik z treścią zadania:<br><span class='formDescription'>Wymagane rozszeżenie pdf</span></td><td>Przesłane: <a href='zadania/".$zadanie->link."' target='_blank'>".$zadanie->link."</a><br><input name='mod_zad_plik' type='file'></td></tr>";	
					echo "</table><input type='submit' value='Modyfikuj' > <input type='reset' value='Resetuj' > <a href='index.php?kat_id=".$_GET['kat_id']."' >Anuluj</a></form><br>";
				}
			}
			else
				echo "Przykro mi, ale niestety zadanie o id: ".$_GET['modify_zad_id']." nie istnieje."; 
		}
		
		 
	}
?>