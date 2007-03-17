#! /usr/bin/ruby -w

require "rexml/document"

class Dialog
  attr_accessor :next
  
  def initialize(name_, text_, next_)
    @name = name_
    @text = text_
    @next = next_
  end

  def show()
    puts "#{@name}: #{@text}" 
    $stdin.readline
    return @next
  end
end

class ChoiceList
  attr_accessor :choices

  def initialize()
    @choices = []
  end
  
  def show()
    choices.each_with_index{|choice, i|
      puts "#{i}.) #{choice.text}"
    }
    puts ""
    begin 
      print "# "
      $stdout.flush
      j = Integer($stdin.readline)
    end until (j.is_a?(Integer) and (j >= 0 and j < choices.length))

    return choices[j].next
  end
end

class Choice
  attr_accessor :text, :next
  def initialize(arg_text, arg_next)
    @text = arg_text
    @next = arg_next
  end
end

conversations = {}

def run(conversations, start)
  if start then
    nxt = conversations[start].show()
    run(conversations, nxt)
  else
    puts "\n[END OF DISCUSSION]"
  end
end

if ARGV.length != 1 then
  puts "Usage: #{$0} FILENAME"
else
  file = File.new(ARGV[0])
  doc = REXML::Document.new(file)

  doc.elements.each("/conversation/*"){|el|
    id = el.attributes["id"]

    if conversations.has_key?(id) then
      puts "Warning: Duplicate conversation id: #{id}"
    end

    case el.name
      when "dialog-list"
      idx = 0
      last_dialog = nil
      el.elements.each("dialog") { |i|
        dialog = Dialog.new(i.elements["name"].text,
                            i.elements["text"].text, 
                            (if i.elements["next"] then
                               i.elements["next"].text
                             else
                               nil
                             end))
        if idx == 0 then
          conversations[id] = dialog
        else
          conversations["#{id}-#{idx}"] = dialog
        end

        if last_dialog then
          last_dialog.next = "#{id}-#{idx}"
        end

        last_dialog = dialog
        idx += 1
      }

      when "dialog"
      dialog = Dialog.new(el.elements["name"].text,
                          el.elements["text"].text, 
                          el.elements["next"].text)
      conversations[id] = dialog
            
      when "choice-list"
      choice_list = ChoiceList.new()
      el.elements.each("choice") { |i|
        choice_list.choices.push(Choice.new(i.elements["text"].text,
                                            if i.elements["next"] then
                                              i.elements["next"].text
                                            else
                                              nil
                                            end))
      }
      conversations[id] = choice_list
    end
  }
  run(conversations, "start")
end

# EOF #
