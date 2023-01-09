<!DOCTYPE html>
 
<html>
<body>
    <?php
        function is_operator($c) {
            $ascii = ord($c);
            return $c == '!' || (($ascii >= 35 && $ascii <= 47) && $c != '\'') || ($ascii >= 58 && $ascii <= 64) || ($ascii >= 91 && $ascii <= 96) || ($ascii >= 123 && $ascii <= 126);
        }

        function read_file($file_name) {
            $lines = array();

            $file = fopen("Files/$file_name", "r") or die("Unable to open file!");

            while(!feof($file)) {
                array_push($lines, fgets($file));
            }   
            
            fclose($file);

            return implode('', $lines);
        }

        function to_tokens($source) {
            $tokens = array();
            $word = '';

            $is_quoting = false;

            $len = strlen($source);

            for($i = 0; $i < $len; $i++) {
                $c = $source[$i];

                // If 'c' is a start of a comment
                if ($c == '#') {
                    while ($i < $len && $source[++$i] != '\n');
                }
                // If '$c' is the opening/closing of quotation
                else if ($c == '"' || $c == '\'') {
                    $is_quoting = !$is_quoting;
                
                    if(!$is_quoting)
                        $word .= $c;
                
                    if (strlen($word) > 0)
                    {
                        array_push($tokens, $word);
                        $word = '';
                    }
                
                    if ($is_quoting)
                        $word .= $c;
                }
                else if ($is_quoting) {
                    $word .= $c;
                }
                // If '$c' is a whitespace
                else if ($c == ' ' || $c == '\n' || ord($c) == 13 || ord($c) == 10) {
                    if (strlen($word) > 0) {
                        array_push($tokens, $word);
                        $word = '';
                    }
                }
                // If '$c' is an operator or is quoting
                else if (is_operator($c)) {
                    if (strlen($word) > 0) {
                        array_push($tokens, $word);
                        $word = '';
                    }

                    array_push($tokens, $c);
                }
                // If it is an ordinary character (a-z, A-Z, 0-9)
                else {
                    $word .= $c;
                }
            }
        
            return $tokens;
        }

        function similarity($tokens_a, $tokens_b) {
            $token_count_a = array();
            $token_count_b = array();

            foreach ($tokens_a as $tok) {
                $ntok = $tok."_";
                
                if (array_key_exists($ntok, $token_count_a)) {
                    $token_count_a[$ntok] += 1;
                } else {
                    $token_count_a[$ntok] = 0;
                }
            }
            foreach ($tokens_b as $tok) {
                $ntok = $tok."_";

                if (array_key_exists($ntok, $token_count_b)) {
                    $token_count_b[$ntok] += 1;
                } else {
                    $token_count_b[$ntok] = 0;
                }
            }
        
            $allTokenCounts = count($token_count_a);
            $matchingTokenCounts = 0;
        
            $steps = 0;

            foreach ($token_count_a as $token => $count) {
                $steps++;
                if (array_key_exists($token, $token_count_b)) {
                    if ($token_count_b[$token] == $count) {
                        $matchingTokenCounts++;
                    }
                }
            }

            return $matchingTokenCounts / $allTokenCounts;
        }

        $file_name_a = 'D10.txt';
        $file_name_b = 'D10Plagiat.txt';

        $tokens_a = to_tokens(read_file($file_name_a));
        $tokens_b = to_tokens(read_file($file_name_b));

        $similarity = similarity($tokens_a, $tokens_b);
        echo "<h1>Podobienstwo dla \"$file_name_a\" i \"$file_name_b\": $similarity</h1>";
    ?>
</body>
</html>