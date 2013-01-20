import Debug.Trace(trace)

parseEntry :: [String] -> (String, [String], [String])
parseEntry (_dash : pid : img : feat : rows : cols : dt : dta : rest) =
    (pid', [img, feat, rows, cols, dt, dta], rest) where
        pid' = drop 15 pid

walk :: [String] -> [(String,[[String]])] -> [(String,[[String]])]
walk [] s = s
walk c  s =
    let (p,i,r) = parseEntry c in
    walk r $ case s of 
               [] -> 
                   [(p,[i])]
               ((sx,ss):sr) 
                   | sx == p   -> ((sx,i:ss):sr)
                   | otherwise -> ((p ,[i] ):s)
    
output_heads :: [(String, [[String]])] -> String
output_heads =
    concat . map (\(pid,info:_) ->
             "   -\n      postid: \"" ++ pid ++ "\n" ++ (unlines info))

output_tails :: [(String, [[String]])] -> String
output_tails =
    concat . map (\(pid,_:infos) ->
         unlines $ map (\info ->
             "   -\n      postid: \"" ++ pid ++ "\n" ++ (unlines info)) infos)

main = do
  all <- getContents
  let info = walk (lines all) []
  trace (output_heads info) $ trace " TRAINSET\n" $ trace (output_tails info) $ print "foo"
  
